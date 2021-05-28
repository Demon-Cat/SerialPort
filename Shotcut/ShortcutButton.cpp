#include "ShortcutButton.h"
#include <QPainter>

ShortcutButton::ShortcutButton(const ShortcutInfo &info, QWidget *parent)
    : QPushButton(parent)
    , m_info(info)
{
    setInfo(info);

    connect(this, &ShortcutButton::clicked, this, &ShortcutButton::onClicked);
}

void ShortcutButton::setInfo(const ShortcutInfo &info)
{
    m_info = info;

    setText(m_info.title());
    setToolTip(m_info.commond());
}

QSize ShortcutButton::sizeHint() const
{
    QFontMetrics fm(font());
    return QSize(fm.horizontalAdvance(text()) + 35, 23);
}

void ShortcutButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.save();
    switch (m_state) {
    case StateNone:
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        break;
    case StateHover:
        painter.setPen(QColor(122, 167, 199, 255));
        painter.setBrush(QColor(122, 167, 199, 80));
        break;
    case StatePressed:
        painter.setPen(QColor(122, 167, 199, 255));
        painter.setBrush(QColor(122, 167, 199, 160));
        break;
    }
    painter.drawRect(rect());
    painter.restore();

    painter.save();
    QRect colorRect(0, 0, 17, 17);
    colorRect.moveCenter(QPoint(15, height() / 2));
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_info.color());
    painter.drawEllipse(colorRect);
    painter.restore();

    painter.save();
    QRect textRect = rect();
    textRect.setLeft(20);
    painter.drawText(textRect, Qt::AlignCenter, text());
    painter.restore();
}

void ShortcutButton::enterEvent(QEvent *event)
{
    m_state = StateHover;
    QPushButton::enterEvent(event);
}

void ShortcutButton::leaveEvent(QEvent *event)
{
    m_state = StateNone;
    QPushButton::leaveEvent(event);
}

void ShortcutButton::mousePressEvent(QMouseEvent *e)
{
    m_state = StatePressed;
    QPushButton::mousePressEvent(e);
}

void ShortcutButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_state = StateHover;
    QPushButton::mouseReleaseEvent(e);
}

void ShortcutButton::onClicked()
{
    QString commond = m_info.commond();
    commond.replace("\\r", "\r");
    emit input(commond.toUtf8());
}
