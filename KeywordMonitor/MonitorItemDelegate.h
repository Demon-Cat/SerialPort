#ifndef MONITORITEMDELEGATE_H
#define MONITORITEMDELEGATE_H

#include <QStyledItemDelegate>

class MonitorItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum MouseState {
        StateNone,
        StateHover,
        StatePressed,
        StateReleased
    };

    MonitorItemDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QRect checkBoxRect(const QStyleOptionViewItem &option) const;

private:
    MouseState m_mouseState = StateNone;
};

#endif // MONITORITEMDELEGATE_H
