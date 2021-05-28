#include "BaseTableData.h"

BaseTableData::BaseTableData()
{

}

BaseTableData::~BaseTableData()
{

}

QVariant BaseTableData::data(int column, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return displayData(column);
    default:
        break;
    }
    return QVariant();
}

void BaseTableData::setData(int column, const QVariant &value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
        setDisplayData(column, value);
        break;
    default:
        break;
    }
}

QVariant BaseTableData::displayData(int column) const
{
    return QVariant();
}

void BaseTableData::setDisplayData(int column, const QVariant &value)
{

}
