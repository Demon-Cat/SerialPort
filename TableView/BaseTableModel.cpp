#include "BaseTableModel.h"

BaseTableModel::BaseTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

BaseTableModel::~BaseTableModel()
{
    qDeleteAll(m_infos);
    m_infos.clear();
}

void BaseTableModel::reset(const QList<BaseTableData *> &infos)
{
    beginResetModel();

    qDeleteAll(m_infos);
    m_infos.clear();
    m_infos = infos;

    endResetModel();
}

BaseTableData *BaseTableModel::info(int row) const
{
    if (row < m_infos.size()) {
        return m_infos.at(row);
    } else {
        return nullptr;
    }
}

void BaseTableModel::setHorizontalHeaderLabels(const QStringList &labels)
{
    m_headers.clear();
    for (int i = 0; i < labels.size(); ++i) {
        m_headers.insert(i, labels.at(i));
    }
}

QVariant BaseTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        switch (role) {
        case Qt::DisplayRole:
            return m_headers.value(section);
        default:
            break;
        }
    }
    return QVariant();
}

bool BaseTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int BaseTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_infos.size();
}

int BaseTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_headers.size();
}

QVariant BaseTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        break;
    }

    if (index.row() < m_infos.size()) {
        BaseTableData *info = m_infos.at(index.row());
        return info->data(index.column(), role);
    } else {
        return QVariant();
    }
}

bool BaseTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        if (index.row() < m_infos.size()) {
            BaseTableData *info = m_infos.at(index.row());
            info->setData(index.column(), value, role);
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags BaseTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

bool BaseTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool BaseTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool BaseTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        BaseTableData *info = m_infos.takeAt(row);
        delete info;
    }

    endRemoveRows();
    return true;
}

bool BaseTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
