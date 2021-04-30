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

signals:

};

#endif // BASETABLEVIEW_H
