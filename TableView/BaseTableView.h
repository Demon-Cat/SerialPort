#ifndef BASETABLEVIEW_H
#define BASETABLEVIEW_H

#include <QTableView>
#include "BaseTableModel.h"

class BaseTableView : public QTableView
{
    Q_OBJECT
public:
    explicit BaseTableView(QWidget *parent = nullptr);
    ~BaseTableView() override;

    int selectedRow() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(int row, int column);
    void doubleClicked(int row, int column);

private slots:
    void onClicked(const QModelIndex &index);
    void onDoubleClicked(const QModelIndex &index);
};

#endif // BASETABLEVIEW_H
