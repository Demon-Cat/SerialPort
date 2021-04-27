#include "OutputTextEdit.h"
#include "ansiescapecodehandler.h"
#include <QScrollBar>
#include <QMenu>
#include <QCursor>
#include <QtDebug>

OutputTextEdit::OutputTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //setReadOnly(true);
    setLineWrapMode(NoWrap);

    m_menu = new QMenu(this);
    QAction *actionCopy = new QAction("复制", this);
    connect(actionCopy, &QAction::triggered, this, &OutputTextEdit::onActionCopy);
    QAction *actionSelectAll = new QAction("选择全部", this);
    connect(actionSelectAll, &QAction::triggered, this, &OutputTextEdit::onActionSelectAll);
    m_menu->addAction(actionCopy);
    m_menu->addSeparator();
    m_menu->addAction(actionSelectAll);

    m_cursor = textCursor();
    m_ansiHandler = new AnsiEscapeCodeHandler();

    m_scrollTimer.setInterval(10);
    m_scrollTimer.setSingleShot(true);
    connect(&m_scrollTimer, &QTimer::timeout, this, &OutputTextEdit::scrollToBottom);
    m_lastMessage.start();

    QFont font("Source Code Pro", 9);
    setFont(font);
}

OutputTextEdit::~OutputTextEdit()
{
    delete m_ansiHandler;
    m_ansiHandler = nullptr;
}

void OutputTextEdit::appendMessage(const QString &out)
{
    const bool atBottom = isScrollbarAtBottom() || m_scrollTimer.isActive();

    if (!m_cursor.atEnd()) {
        m_cursor.movePosition(QTextCursor::End);
    }

    QList<FormattedText> formattedTextList = m_ansiHandler->parseText(FormattedText(out));
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

void OutputTextEdit::setLocalEchoEnable(bool enable)
{
    m_localEchoEnabled = enable;
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

void OutputTextEdit::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if (m_localEchoEnabled) {
            QPlainTextEdit::keyPressEvent(event);
        }
        moveCursorToEnd();
        emit getData(event->text().toLocal8Bit());
    }
}

void OutputTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)

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

void OutputTextEdit::onActionSelectAll()
{
    selectAll();
}
