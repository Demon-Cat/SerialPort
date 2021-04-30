#ifndef MONITORITEMMODEL_H
#define MONITORITEMMODEL_H

#include <QAbstractTableModel>
#include "MonitorInfo.h"

class MonitorItemModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIndex {
        ColumnEnable,
        ColumnType,
        ColumnCase,
        ColumnRobot,
        ColumnText
    };

    explicit MonitorItemModel(QObject *parent = nullptr);
    ~MonitorItemModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QMap<int, QString> m_headers;
    QList<MonitorInfo *> m_infos;
};

#endif // MONITORITEMMODEL_H
