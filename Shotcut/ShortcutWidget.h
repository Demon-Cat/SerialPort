#ifndef SHORTCUTWIDGET_H
#define SHORTCUTWIDGET_H

#include <QWidget>
#include <QMenu>

class ShortcutButton;

class ShortcutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShortcutWidget(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void updateLayout();

private slots:
    void onActionNewButton();

signals:
    void input(const QByteArray &data);

private:
    QMenu *m_menu = nullptr;
    QAction *m_actionNewButton = nullptr;

    QList<ShortcutButton *> m_shortcutList;
};

#endif // SHORTCUTWIDGET_H
