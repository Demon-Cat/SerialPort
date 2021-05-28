#include "ShortcutWidget.h"
#include "AddButton.h"
#include "Database.h"
#include "ShortcutButton.h"
#include <QCursor>
#include <QtDebug>

const int ButtonHeight = 30;

ShortcutWidget::ShortcutWidget(QWidget *parent)
    : QWidget(parent)
{
    m_menu = new QMenu(this);
    m_actionNewButton = new QAction("添加快捷发送", this);
    connect(m_actionNewButton, &QAction::triggered, this, &ShortcutWidget::onActionNewButton);
    m_menu->addAction(m_actionNewButton);

    QMap<int, ShortcutButton *> shortcutMap;
    QList<ShortcutInfo> infoList = gDatabase.selectShortcut();
    foreach (const auto &info, infoList) {
        int index = info.id();
        if (index < 0) {
            qWarning() << "index:" << index << "invalid";
            continue;
        }
        if (shortcutMap.contains(index)) {
            qWarning() << "index:" << index << "already existed";
            continue;
        }
        ShortcutButton *button = new ShortcutButton(info, this);
        connect(button, &ShortcutButton::input, this, &ShortcutWidget::input);
        shortcutMap.insert(info.id(), button);
    }
    for (auto iter = shortcutMap.constBegin(); iter != shortcutMap.constEnd(); ++iter) {
        m_shortcutList.append(iter.value());
    }
}

void ShortcutWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event)

    m_menu->exec(QCursor::pos());
}

void ShortcutWidget::resizeEvent(QResizeEvent *event)
{
    updateLayout();
    QWidget::resizeEvent(event);
}

void ShortcutWidget::showEvent(QShowEvent *event)
{
    updateLayout();
    QWidget::showEvent(event);
}

void ShortcutWidget::updateLayout()
{
    QRect rc = rect();
    QPoint center = rc.center();

    int x = 0;
    for (int i = 0; i < m_shortcutList.size(); ++i) {
        ShortcutButton *button = m_shortcutList.at(i);
        int w = button->sizeHint().width();
        button->setGeometry(x, center.y() - ButtonHeight / 2, w, ButtonHeight);
        button->show();
        x += w;
    }
}

void ShortcutWidget::onActionNewButton()
{
    AddButton add(this);
    if (add.exec() == AddButton::Accepted) {
        ShortcutInfo info = add.info();
        info.setId(m_shortcutList.size());
        ShortcutButton *button = new ShortcutButton(info, this);
        connect(button, &ShortcutButton::input, this, &ShortcutWidget::input);
        m_shortcutList.append(button);
        updateLayout();

        gDatabase.updateShortcut(info);
    }
}
