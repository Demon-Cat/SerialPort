#include "RobotInfo.h"

RobotInfo::RobotInfo()
    : BaseTableData()
{
}

int RobotInfo::id() const
{
    return m_id;
}

void RobotInfo::setId(int newId)
{
    m_id = newId;
}

const QString &RobotInfo::name() const
{
    return m_name;
}

void RobotInfo::setName(const QString &newName)
{
    m_name = newName;
}

const QString &RobotInfo::webhook() const
{
    return m_webhook;
}

void RobotInfo::setWebhook(const QString &newWebhook)
{
    m_webhook = newWebhook;
}

const QString &RobotInfo::note() const
{
    return m_note;
}

void RobotInfo::setNote(const QString &newNote)
{
    m_note = newNote;
}

QVariant RobotInfo::displayData(int column) const
{
    switch (column) {
    case ColumnId:
        return id();
    case ColumnName:
        return name();
    case ColumnWebhook:
        return webhook();
    case ColumnNote:
        return note();
    default:
        break;
    }
    return QVariant();
}
