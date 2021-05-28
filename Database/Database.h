#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "RobotInfo.h"
#include "SerialPortInfo.h"
#include "ShortcutInfo.h"

#define gDatabase Database::instance()

class Database : public QObject
{
    Q_OBJECT
public:
    enum DataBaseError {
        NoError,
        QueryError,
        TransactionError,
        CommitError,

        RobotNameExist,
        RobotWebhookExist
    };

    explicit Database(QObject *parent = nullptr);

    static Database &instance();

    //serial port
    int updateSerialPort(const SerialPortInfo &info) const;
    int deleteSerialPort(const SerialPortInfo &info) const;
    QList<SerialPortInfo> selectSerialPort() const;

    //robot
    int insertRobot(const RobotInfo &info) const;
    int updateRobot(const RobotInfo &info) const;
    int deleteRobot(const RobotInfo &info) const;
    QList<RobotInfo *> selectRobot() const;

    //shortcut
    int updateShortcut(const ShortcutInfo &info) const;
    int updateShortcut(const QList<ShortcutInfo> &infoList) const;
    int deleteShortcut(const ShortcutInfo &info) const;
    QList<ShortcutInfo> selectShortcut() const;

signals:

private:

};

#endif // DATABASE_H
