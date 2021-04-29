#include "InputTextEdit.h"

InputTextEdit::InputTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setWordWrapMode(QTextOption::NoWrap);
}

void InputTextEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit enterPressed();
        break;
    default:
        QPlainTextEdit::keyPressEvent(e);
        break;
    }
}
