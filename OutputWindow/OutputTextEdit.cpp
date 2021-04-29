#include "OutputTextEdit.h"
#include <QGuiApplication>
#include "ansiescapecodehandler.h"
#include <QClipboard>
#include <QCursor>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <QScrollBar>
#include <QtDebug>

OutputTextEdit::OutputTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //setReadOnly(true);
    setLineWrapMode(NoWrap);
    setOverwriteMode(true);
    //setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    m_menu = new QMenu(this);
    m_actionCopy = new QAction("复制", this);
    connect(m_actionCopy, &QAction::triggered, this, &OutputTextEdit::onActionCopy);
    m_actionCopyAll = new QAction("复制全部", this);
    connect(m_actionCopyAll, &QAction::triggered, this, &OutputTextEdit::onActionCopyAll);
    m_actionSelectAll = new QAction("选择全部", this);
    connect(m_actionSelectAll, &QAction::triggered, this, &OutputTextEdit::onActionSelectAll);
    m_menu->addAction(m_actionCopy);
    m_menu->addAction(m_actionCopyAll);
    m_menu->addSeparator();
    m_menu->addAction(m_actionSelectAll);

    m_cursor = textCursor();
    m_ansiHandler = new AnsiEscapeCodeHandler();

    m_scrollTimer.setInterval(10);
    m_scrollTimer.setSingleShot(true);
    connect(&m_scrollTimer, &QTimer::timeout, this, &OutputTextEdit::scrollToBottom);
    m_lastMessage.start();
}

OutputTextEdit::~OutputTextEdit()
{
    delete m_ansiHandler;
    m_ansiHandler = nullptr;
}

void OutputTextEdit::appendMessage(const QString &text)
{
    const bool atBottom = isScrollbarAtBottom() || m_scrollTimer.isActive();

    if (!m_cursor.atEnd()) {
        m_cursor.movePosition(QTextCursor::End);
    }

    QList<FormattedText> formattedTextList = m_ansiHandler->parseText(FormattedText(text));
    for (int i = 0; i < formattedTextList.size(); ++i) {
        const FormattedText &formattedText = formattedTextList.at(i);

        QTextCursor cursor = textCursor();
        if (overwriteMode() && !cursor.hasSelection() && !cursor.atBlockEnd()) {
            // no need to call deleteChar() if we have a selection, insertText does it already
            cursor.deleteChar();
        }
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

void OutputTextEdit::insertMessage(const QString &text, bool keepCursor)
{
    QTextCursor cursor = textCursor();
    int position = cursor.position();
    cursor.insertText(text);
    cursor.setPosition(position);
    if (keepCursor) {
        setTextCursor(cursor);
    }
}

void OutputTextEdit::overwriteMessage(const QString &text)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, text.size());
    cursor.insertText(text);
}

void OutputTextEdit::setLocalEchoEnable(bool enable)
{
    m_localEchoEnabled = enable;
}

void OutputTextEdit::deletePreviousChar()
{
    QTextCursor cursor = textCursor();
    cursor.deletePreviousChar();
}

/**
 * @brief OutputTextEdit::deleteAfterChars
 * 删除从光标之前1个字符到最后的所有字符
 */
void OutputTextEdit::deleteAfterChars()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
}

void OutputTextEdit::movePosition(QTextCursor::MoveOperation operation)
{
    switch (operation) {
    case QTextCursor::Left:
    case QTextCursor::Right: {
        QTextCursor cursor = textCursor();
        cursor.movePosition(operation);
        setTextCursor(cursor);
        break;
    }
    default:
        break;
    }
}

bool OutputTextEdit::isEmpty() const
{
    return document()->isEmpty();
}

bool OutputTextEdit::isScrollbarAtBottom() const
{
    return verticalScrollBar()->value() == verticalScrollBar()->maximum();
}

void OutputTextEdit::moveCursorToEnd()
{
    QTextCursor cursor = textCursor();
    if (!cursor.atEnd()) {
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }
}

void OutputTextEdit::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);

#if 0
    QPainter painter(viewport());
    painter.fillRect(cursorRect(), QBrush(QColor(0, 0, 255, 80)));
#endif
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
    QByteArray text;
    switch (event->key()) {
    case Qt::Key_Backspace:
        //先发送0x08过去，根据收到的回复执行退格操作
        text = event->text().toLocal8Bit();
        break;
    case Qt::Key_Left:
        text = QByteArray::fromHex("1B5B44");
        break;
    case Qt::Key_Right:
        text = QByteArray::fromHex("1B5B43");
        break;
    case Qt::Key_Up:
        text = QByteArray::fromHex("1B5B41");
        moveCursorToEnd();
        break;
    case Qt::Key_Down:
        text = QByteArray::fromHex("1B5B42");
        moveCursorToEnd();
        break;
    default:
        text = event->text().toLocal8Bit();
        moveCursorToEnd();
        break;
    }

    if (m_localEchoEnabled) {
        QPlainTextEdit::keyPressEvent(event);
    }
    if (!text.isEmpty()) {
        emit getData(text);
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
    m_menu->exec(QCursor::pos());
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

void OutputTextEdit::onActionSelectAll()
{
    selectAll();
}
