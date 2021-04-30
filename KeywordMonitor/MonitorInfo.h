#ifndef MONITORINFO_H
#define MONITORINFO_H

#include <QString>
#include <QMetaType>

class MonitorInfo {
public:
    enum MatchType {
        MatchString,
        MatchRegular
    };

    MonitorInfo();
    virtual ~MonitorInfo();

    QVariant data(int column, int role) const;
    void setData(int column, const QVariant &value, int role);

    bool enable() const;
    void setEnable(bool enable);

    MatchType type() const;
    void setType(const MatchType &type);
    QString typeString() const;

    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(const Qt::CaseSensitivity &caseSensitivity);
    QString caseSensitivityString() const;

    QString robot() const;
    void setRobot(const QString &robot);

    QString text() const;
    void setText(const QString &text);

private:
    bool m_enable = true;
    MatchType m_type = MatchString;
    Qt::CaseSensitivity m_caseSensitivity = Qt::CaseSensitive;
    QString m_robot;
    QString m_text;
};

Q_DECLARE_METATYPE(MonitorInfo::MatchType)

#endif // MONITORINFO_H
