#ifndef ROBOTINFO_H
#define ROBOTINFO_H

#include "BaseTableData.h"

class RobotInfo : public BaseTableData {
public:
    enum Column {
        ColumnId,
        ColumnName,
        ColumnWebhook,
        ColumnNote
    };

    RobotInfo();

    int id() const;
    void setId(int newId);

    const QString &name() const;
    void setName(const QString &newName);

    const QString &webhook() const;
    void setWebhook(const QString &newWebhook);

    const QString &note() const;
    void setNote(const QString &newNote);

protected:
    QVariant displayData(int column) const override;

private:
    int m_id = -1;
    QString m_name;
    QString m_webhook;
    QString m_note;
};

#endif // ROBOTINFO_H
