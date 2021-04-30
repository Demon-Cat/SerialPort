#include "MonitorInfo.h"
#include "MonitorItemModel.h"

MonitorInfo::MonitorInfo()
{
}

MonitorInfo::~MonitorInfo()
{

}

QVariant MonitorInfo::data(int column, int role) const
{
    switch (role) {
    case Qt::DisplayRole: {
        switch (column) {
        case MonitorItemModel::ColumnType:
            return typeString();
        case MonitorItemModel::ColumnCase:
            return caseSensitivityString();
        case MonitorItemModel::ColumnRobot:
            return robot();
        case MonitorItemModel::ColumnText:
            return text();
        }
        break;
    }
    case Qt::UserRole: {
        switch (column) {
        case MonitorItemModel::ColumnEnable:
            return enable();
        case MonitorItemModel::ColumnType:
            return type();
        case MonitorItemModel::ColumnCase:
            return caseSensitivity();
        case MonitorItemModel::ColumnRobot:
            return robot();
        case MonitorItemModel::ColumnText:
            return text();
        }
        break;
    }
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignCenter);
    default:
        break;
    }

    return QVariant();
}

void MonitorInfo::setData(int column, const QVariant &value, int role)
{
    switch (role) {
    case Qt::DisplayRole: {
        switch (column) {
        case MonitorItemModel::ColumnEnable:
            setEnable(value.toBool());
            break;
        case MonitorItemModel::ColumnType:
            setType(value.value<MonitorInfo::MatchType>());
            break;
        case MonitorItemModel::ColumnCase:
            setCaseSensitivity(value.value<Qt::CaseSensitivity>());
            break;
        case MonitorItemModel::ColumnRobot:
            setRobot(value.toString());
            break;
        case MonitorItemModel::ColumnText:
            setText(value.toString());
            break;
        default:
            break;
        }
        break;
    }
    case Qt::UserRole: {
        switch (column) {
        case MonitorItemModel::ColumnEnable:
            setEnable(value.toBool());
            break;
        case MonitorItemModel::ColumnType:
            break;
        case MonitorItemModel::ColumnCase:
            break;
        case MonitorItemModel::ColumnRobot:
            break;
        case MonitorItemModel::ColumnText:
            break;
        default:
            break;
        }
        break;
    }
    case Qt::EditRole: {
        switch (column) {
        case MonitorItemModel::ColumnEnable:
            break;
        case MonitorItemModel::ColumnType:
            setType(value.value<MonitorInfo::MatchType>());
            break;
        case MonitorItemModel::ColumnCase:
            setCaseSensitivity(value.value<Qt::CaseSensitivity>());
            break;
        case MonitorItemModel::ColumnRobot:
            break;
        case MonitorItemModel::ColumnText:
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

bool MonitorInfo::enable() const
{
    return m_enable;
}

void MonitorInfo::setEnable(bool enable)
{
    m_enable = enable;
}

MonitorInfo::MatchType MonitorInfo::type() const
{
    return m_type;
}

void MonitorInfo::setType(const MatchType &type)
{
    m_type = type;
}

QString MonitorInfo::typeString() const
{
    QString text;
    switch (type()) {
    case MatchString:
        text = "字符串匹配";
        break;
    case MatchRegular:
        text = "正则匹配";
        break;
    }
    return text;
}

Qt::CaseSensitivity MonitorInfo::caseSensitivity() const
{
    return m_caseSensitivity;
}

void MonitorInfo::setCaseSensitivity(const Qt::CaseSensitivity &caseSensitivity)
{
    m_caseSensitivity = caseSensitivity;
}

QString MonitorInfo::caseSensitivityString() const
{
    QString text;
    switch (caseSensitivity()) {
    case Qt::CaseSensitive:
        text = "匹配大小写";
        break;
    case Qt::CaseInsensitive:
        text = "不匹配大小写";
        break;
    }
    return text;
}

QString MonitorInfo::robot() const
{
    return m_robot;
}

void MonitorInfo::setRobot(const QString &robot)
{
    m_robot = robot;
}

QString MonitorInfo::text() const
{
    return m_text;
}

void MonitorInfo::setText(const QString &text)
{
    m_text = text;
}
