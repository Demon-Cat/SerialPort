#ifndef ROBOTTABLEMODEL_H
#define ROBOTTABLEMODEL_H

#include "BaseTableModel.h"
#include "RobotInfo.h"

class RobotTableModel : public BaseTableModel
{
    Q_OBJECT

    enum HeaderColumn {
        ColumnName,
        Column
    };

public:
    explicit RobotTableModel(QObject *parent = nullptr);

    void reset(const QList<RobotInfo *> &infos);

    Qt::ItemFlags flags(const QModelIndex& index) const override;

signals:

};

#endif // ROBOTTABLEMODEL_H
