#ifndef MONITORTABLEVIEW_H
#define MONITORTABLEVIEW_H

#include <QTableView>
#include "MonitorItemModel.h"
#include "MonitorItemDelegate.h"

class MonitorTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MonitorTableView(QWidget *parent = nullptr);

    void appendRow();

signals:

private:
    MonitorItemModel *m_model = nullptr;
    MonitorItemDelegate * m_delegate = nullptr;
};

#endif // MONITORTABLEVIEW_H
