#include "RobotTableModel.h"
#include "RobotInfo.h"

RobotTableModel::RobotTableModel(QObject *parent)
    : BaseTableModel(parent)
{
    QStringList headers;
    headers << "编号";
    headers << "机器人名字";
    headers << "Webhook地址";
    headers << "备注";
    setHorizontalHeaderLabels(headers);
}

void RobotTableModel::reset(const QList<RobotInfo *> &infos)
{
    QList<BaseTableData *> list;
    for (int i = 0; i < infos.size(); ++i) {
        list.append(infos.at(i));
    }
    BaseTableModel::reset(list);
}

Qt::ItemFlags RobotTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
