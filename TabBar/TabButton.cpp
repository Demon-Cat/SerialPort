#include "TabButton.h"
#include <QPainter>

const int IconWidth = 16;
const int CloseWidth = 16;

TabButton::TabButton(QWidget *parent)
    : QToolButton(parent)
{
    setCheckable(true);

    connect(this, &TabButton::clicked, this, &TabButton::onClicked);

    m_closeButton = new QToolButton(this);
    m_closeButton->setIconSize(QSize(8, 8));
    m_closeButton->setIcon(QIcon("://images/close.png"));
    m_closeButton->setStyleSheet("QToolButton{width:16px;height:16px;border:0px;}QToolButton:hover{border-radius:8px;background:#20000000}");
    connect(m_closeButton, &QToolButton::clicked, this, &TabButton::onCloseClicked);
}

void TabButton::resizeEvent(QResizeEvent *event)
{
    QRect rc = rect();
    QPoint center = rc.center();
    m_closeButton->setGeometry(rc.right() - 5 - 16, center.y() - 8, 16, 16);

    QToolButton::resizeEvent(event);
}

void TabButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //background
    painter.save();
    painter.setPen(Qt::NoPen);
    if (isChecked()) {
        painter.setBrush(QColor(255, 255, 255));
    } else {
        painter.setBrush(Qt::NoBrush);
    }
    painter.drawRect(rect());
    painter.restore();

    //border-right
    painter.save();
    painter.setPen(QColor(217, 217, 217));
    painter.drawLine(rect().topRight(), rect().bottomRight());
    painter.restore();

    //icon
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QRect iconRc(0, 0, qMin(IconWidth, height()), qMin(IconWidth, height()));
    iconRc.moveCenter(rect().center());
    iconRc.moveLeft(5);
    switch (m_state) {
    case StateConnected:
        painter.drawPixmap(iconRc, QPixmap("://images/check.png"));
        break;
    case StateDisconnected:
        painter.drawPixmap(iconRc, QPixmap("://images/forbid.png"));
        break;
    default:
        break;
    }

    painter.restore();

    //text
    painter.save();
    QRect textRc = rect();
    textRc.setLeft(iconRc.right());
    textRc.setRight(width() - CloseWidth);
    painter.drawText(textRc, Qt::AlignCenter, text());
    painter.restore();
}

void TabButton::onClicked()
{
    emit tabClicked(text());
}

void TabButton::onCloseClicked()
{
    emit tabCloseRequested(text());
}

TabButton::State TabButton::state() const
{
    return m_state;
}

void TabButton::setState(State newState)
{
    m_state = newState;
    update();
}
