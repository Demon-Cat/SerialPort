#include "TabBar.h"
#include "TabButton.h"
#include <QPainter>
#include <QtDebug>

const int TabHeight = 30;
const int CloseButtonDiameter = 26;
const int TabButtonWidth = 100;

TabBar::TabBar(QWidget *parent)
    : QWidget(parent)
{
    m_addButton = new QToolButton(this);
    m_addButton->setIconSize(QSize(12, 12));
    m_addButton->setIcon(QIcon("://images/add.png"));
    m_addButton->setStyleSheet(QString("QToolButton{width:%1px;height:%1px;border:0px;}QToolButton:hover{border-radius:%2px;background:#20000000}")
                                   .arg(CloseButtonDiameter)
                                   .arg(CloseButtonDiameter / 2));
    connect(m_addButton, &QToolButton::clicked, this, &TabBar::onAddClicked);
}

int TabBar::count() const
{
    return m_mapButton.count();
}

int TabBar::addTab(const QString &text)
{
    if (m_mapButton.contains(text)) {
        return -1;
    }
    TabButton *button = new TabButton(this);
    button->setText(text);
    button->show();
    connect(button, &TabButton::tabClicked, this, &TabBar::onTabClicked);
    connect(button, &TabButton::tabCloseRequested, this, &TabBar::onTabCloseRequested);
    m_mapButton.insert(text, button);

    updateLayout();
    return 0;
}

void TabBar::removeTab(const QString &text)
{
    if (!m_mapButton.contains(text)) {
        return;
    }
    TabButton *button = m_mapButton.value(text);
    button->deleteLater();
    m_mapButton.remove(text);
    updateLayout();
    if (!m_mapButton.isEmpty()) {
        setCurrentTab(m_mapButton.firstKey());
    }
}

void TabBar::setTabState(const QString &text, TabButton::State state)
{
    if (m_mapButton.contains(text)) {
        TabButton *button = m_mapButton.value(text);
        button->setState(state);
    }
}

QString TabBar::currentTab() const
{
    QString text;
    for (auto iter = m_mapButton.constBegin(); iter != m_mapButton.constEnd(); ++iter) {
        TabButton *button = iter.value();
        if (button->isChecked()) {
            text = button->text();
            break;
        }
    }
    return text;
}

void TabBar::setCurrentTab(const QString &text)
{
    bool find = false;
    for (auto iter = m_mapButton.constBegin(); iter != m_mapButton.constEnd(); ++iter) {
        TabButton *button = iter.value();
        if (button->text() == text) {
            find = true;
            button->setChecked(true);
        } else {
            button->setChecked(false);
        }
    }

    if (find) {
        emit tabClicked(text);
    }
}

QSize TabBar::sizeHint() const
{
    return QSize(200, TabHeight);
}

void TabBar::resizeEvent(QResizeEvent *event)
{
    updateLayout();
    QWidget::resizeEvent(event);
}

void TabBar::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void TabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(242, 242, 242));
    painter.drawRect(rect());

    painter.setPen(QColor(217, 217, 217));
    painter.drawLine(rect().bottomLeft(), rect().bottomRight());
}

void TabBar::updateLayout()
{
    QRect rc = rect();
    QPoint center = rc.center();

    int x = 0;
    for (auto iter = m_mapButton.constBegin(); iter != m_mapButton.constEnd(); ++iter) {
        TabButton *tab = iter.value();
        tab->setGeometry(x, rc.top(), TabButtonWidth, height());
        x += TabButtonWidth;
    }

    m_addButton->setGeometry(m_mapButton.count() * TabButtonWidth + 10, center.y() - CloseButtonDiameter / 2, CloseButtonDiameter, CloseButtonDiameter);
}

void TabBar::onAddClicked()
{
    emit addClicked();
}

void TabBar::onTabClicked(const QString &text)
{
    setCurrentTab(text);
}

void TabBar::onTabCloseRequested(const QString &text)
{
    emit tabCloseRequested(text);
}
