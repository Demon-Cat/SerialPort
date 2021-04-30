#ifndef BASEHEADERDATA_H
#define BASEHEADERDATA_H

#include <QMap>
#include <QVariant>

class BaseHeaderData
{
public:
    BaseHeaderData();
    virtual ~BaseHeaderData();

protected:
    QMap<int, QVariant> m_dataMap;
};

#endif // BASEHEADERDATA_H
