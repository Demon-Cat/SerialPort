#include "InputTextEdit.h"
#include "Settings.h"
#include <QtDebug>

InputTextEdit::InputTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setWordWrapMode(QTextOption::NoWrap);

    setFont(gSettings.sessionFont());
}

void InputTextEdit::keyPressEvent(QKeyEvent *e)
{
    //qDebug() << e;

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
