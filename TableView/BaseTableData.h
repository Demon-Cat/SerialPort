#ifndef BASETABLEDATA_H
#define BASETABLEDATA_H

#include <QVariant>

class BaseTableData
{
public:
    BaseTableData();
    virtual ~BaseTableData();

    QVariant data(int column, int role) const;
    void setData(int column, const QVariant &value, int role);

protected:
    virtual QVariant displayData(int column) const;
    virtual void setDisplayData(int column, const QVariant &value);
};

#endif // BASETABLEDATA_H
