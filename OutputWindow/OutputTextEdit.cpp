#include "OutputTextEdit.h"
#include <QGuiApplication>
#include "Settings.h"
#include "ansiescapecodehandler.h"
#include <QClipboard>
#include <QCursor>
#include <QFontDialog>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <QScrollBar>
#include <QtDebug>

OutputTextEdit::OutputTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setReadOnly(true);
    setTabChangesFocus(false);
    setLineWrapMode(NoWrap);
    setOverwriteMode(true);
    setUndoRedoEnabled(false);

    m_menu = new QMenu(this);
    m_actionCopy = new QAction("复制", this);
    connect(m_actionCopy, &QAction::triggered, this, &OutputTextEdit::onActionCopy);
    m_actionCopyAll = new QAction("复制全部", this);
    connect(m_actionCopyAll, &QAction::triggered, this, &OutputTextEdit::onActionCopyAll);
    m_actionPaster = new QAction("粘贴", this);
    connect(m_actionPaster, &QAction::triggered, this, &OutputTextEdit::onActionPaster);
    m_actionClean = new QAction("清空", this);
    connect(m_actionClean, &QAction::triggered, this, &OutputTextEdit::onActionClean);
    m_actionScrollToBottom = new QAction("滚动到最后", this);
    connect(m_actionScrollToBottom, &QAction::triggered, this, &OutputTextEdit::onActionScrollToBottom);
    m_actionFont = new QAction("字体...", this);
    connect(m_actionFont, &QAction::triggered, this, &OutputTextEdit::onActionFont);
    m_menu->addAction(m_actionCopy);
    m_menu->addAction(m_actionCopyAll);
    m_menu->addAction(m_actionPaster);
    m_menu->addSeparator();
    m_menu->addAction(m_actionClean);
    m_menu->addSeparator();
    m_menu->addAction(m_actionScrollToBottom);
    m_menu->addSeparator();
    m_menu->addAction(m_actionFont);

    m_cursor = textCursor();
    m_ansiHandler = new AnsiEscapeCodeHandler();

    m_scrollTimer.setInterval(10);
    m_scrollTimer.setSingleShot(true);
    connect(&m_scrollTimer, &QTimer::timeout, this, &OutputTextEdit::scrollToBottom);
    m_lastMessage.start();

    m_cursorBlink.setInterval(500);
    connect(&m_cursorBlink, &QTimer::timeout, this, [=]() {
        m_blink = !m_blink;
        viewport()->update();
    });
    m_cursorBlink.start();

    //    QPalette p = palette();
    //    p.setBrush(QPalette::Base, QColor(46, 47, 48));
    //    setPalette(p);

    setFont(gSettings.sessionFont());
}

OutputTextEdit::~OutputTextEdit()
{
    delete m_ansiHandler;
    m_ansiHandler = nullptr;
}

void OutputTextEdit::appendMessage(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    //超过两万行后一次性删除一万行，避免一直滚动
    if (blockCount() > 20000) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor, 10000);
        cursor.removeSelectedText();
    }

    //
    const bool atBottom = isScrollbarAtBottom() || m_scrollTimer.isActive();

    if (!m_cursor.atEnd()) {
        if (text.size() == 1) {
            m_cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        } else {
            m_cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        }
    }

    QList<FormattedText> formattedTextList = m_ansiHandler->parseText(FormattedText(text));
    for (int i = 0; i < formattedTextList.size(); ++i) {
        const FormattedText &formattedText = formattedTextList.at(i);
        m_cursor.insertText(formattedText.text, formattedText.format);
    }

    if (atBottom) {
        if (m_lastMessage.elapsed() < 5) {
            m_scrollTimer.start();
        } else {
            m_scrollTimer.stop();
            scrollToBottom();
        }
    }

    m_lastMessage.start();
}

/**
 * @brief OutputTextEdit::deleteAfterChars
 * 删除从光标到最后的所有字符
 */
void OutputTextEdit::deleteAfterChars()
{
    m_cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    m_cursor.removeSelectedText();
}

void OutputTextEdit::movePosition(QTextCursor::MoveOperation operation)
{
    m_cursor.movePosition(operation);
    setTextCursor(m_cursor);
}

bool OutputTextEdit::isEmpty() const
{
    return document()->isEmpty();
}

bool OutputTextEdit::isScrollbarAtBottom() const
{
    return verticalScrollBar()->value() == verticalScrollBar()->maximum();
}

void OutputTextEdit::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);

    if (!hasFocus()) {
        return;
    }

    if (m_blink) {
        QPainter painter(viewport());
        painter.fillRect(cursorRect(m_cursor), QBrush(QColor(0, 0, 0, 100)));
    }
}

void OutputTextEdit::mousePressEvent(QMouseEvent *e)
{
    QPlainTextEdit::mousePressEvent(e);
}

void OutputTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
    QPlainTextEdit::mouseReleaseEvent(e);
}

void OutputTextEdit::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << event;

    QByteArray text;
    switch (event->key()) {
    case Qt::Key_Backspace:
        //先发送0x08过去，根据收到的回复执行退格操作
        text = "\b";
        break;
    case Qt::Key_Left:
        text = "\x1b[D";
        break;
    case Qt::Key_Right:
        text = "\x1b[C";
        break;
    case Qt::Key_Up:
        text = "\x1b[A";
        break;
    case Qt::Key_Down:
        text = "\x1b[B";
        break;
    case Qt::Key_End:
        text = "\x1b[4~";
        break;
    default:
        text = event->text().toLocal8Bit();
        break;
    }

    if (!text.isEmpty()) {
        emit input(text);
    }
}

void OutputTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)

    if (textCursor().hasSelection()) {
        m_actionCopy->setEnabled(true);
    } else {
        m_actionCopy->setEnabled(false);
    }

    if (isEmpty()) {
        m_actionCopyAll->setEnabled(false);
    } else {
        m_actionCopyAll->setEnabled(true);
    }

    m_menu->exec(QCursor::pos());
}

bool OutputTextEdit::focusNextPrevChild(bool next)
{
    Q_UNUSED(next)
    //避免disable后无法收到tab键消息
    return false;
}

void OutputTextEdit::scrollToBottom()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    // QPlainTextEdit destroys the first calls value in case of multiline
    // text, so make sure that the scroll bar actually gets the value set.
    // Is a noop if the first call succeeded.
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void OutputTextEdit::onActionCopy()
{
    copy();
}

void OutputTextEdit::onActionCopyAll()
{
    QString text = toPlainText();
    QMimeData *data = new QMimeData();
    data->setText(text);
    QGuiApplication::clipboard()->setMimeData(data);
}

void OutputTextEdit::onActionPaster()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString originalText = clipboard->text();
    if (originalText.isEmpty()) {
        return;
    }
    emit input(originalText.toUtf8());
}

void OutputTextEdit::onActionSelectAll()
{
    selectAll();
}

void OutputTextEdit::onActionClean()
{
    clear();
}

void OutputTextEdit::onActionScrollToBottom()
{
    scrollToBottom();
}

void OutputTextEdit::onActionFont()
{
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, font(), this);
    if (ok) {
        setFont(newFont);
        gSettings.saveSessionFont(newFont);
    } else {
    }
}
