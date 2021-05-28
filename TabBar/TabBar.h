#ifndef TABBAR_H
#define TABBAR_H

#include <QWidget>
#include <QMap>
#include "TabButton.h"

class TabBar : public QWidget
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = nullptr);

    int count() const;
    int addTab(const QString &text);
    void removeTab(const QString &text);

    void setTabState(const QString &text, TabButton::State state);

    QString currentTab() const;
    void setCurrentTab(const QString &text);

    QSize sizeHint() const override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void updateLayout();

signals:
    void addClicked();
    void tabClicked(const QString &text);
    void tabCloseRequested(const QString &text);

private slots:
    void onAddClicked();
    void onTabClicked(const QString &text);
    void onTabCloseRequested(const QString &text);

private:
    QMap<QString, TabButton *> m_mapButton;

    QToolButton *m_addButton = nullptr;
};

#endif // TABBAR_H
