#include "SelectColor.h"
#include <QPainter>

SelectColor::SelectColor(QWidget *parent)
    : QToolButton(parent)
{
}

const QColor &SelectColor::color() const
{
    return m_color;
}

void SelectColor::setColor(const QColor &newColor)
{
    m_color = newColor;
    update();
}

void SelectColor::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    int r = qMin(width() / 2, height() / 2) - 5;
    painter.drawEllipse(rect().center(), r, r);
}
