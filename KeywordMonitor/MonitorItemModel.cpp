#include "MonitorItemModel.h"

MonitorItemModel::MonitorItemModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_headers.insert(0, "启用");
    m_headers.insert(1, "匹配方式");
    m_headers.insert(2, "大小写敏感");
    m_headers.insert(3, "发送到群机器人");
    m_headers.insert(4, "文本");
}

MonitorItemModel::~MonitorItemModel()
{
    for (int i = 0; i < m_infos.size(); ++i) {
        MonitorInfo *info = m_infos.at(i);
        delete info;
    }
    m_infos.clear();
}

QVariant MonitorItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            return m_headers.value(section);
        }
    }
    return QVariant();
}

bool MonitorItemModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int MonitorItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_infos.count();
}

int MonitorItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_headers.count();
}

QVariant MonitorItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    MonitorInfo *info = m_infos.at(index.row());
    return info->data(index.column(), role);
}

bool MonitorItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        MonitorInfo *info = m_infos.at(index.row());
        info->setData(index.column(), value, role);
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags MonitorItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

bool MonitorItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        MonitorInfo *info = new MonitorInfo();
        m_infos.insert(row + i, info);
    }
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool MonitorItemModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool MonitorItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        MonitorInfo *info = m_infos.takeAt(row);
        delete info;
    }
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool MonitorItemModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
