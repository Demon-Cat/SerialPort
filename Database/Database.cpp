#include "Database.h"
#include <QApplication>
#include <QElapsedTimer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>

Database::Database(QObject *parent)
    : QObject(parent)
{
    QString filePath = QString("%1/data/data.db").arg(qApp->applicationDirPath());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath);
    bool ok = db.open();
    if (!ok) {
        qWarning() << "数据库打开失败：" << db.lastError();
    }
}

Database &Database::instance()
{
    static Database self;
    return self;
}

int Database::updateSerialPort(const SerialPortInfo &info) const
{
    QSqlQuery query;
    bool ok;

    query.prepare("SELECT * FROM serialport WHERE portname=:portname");
    query.bindValue(":portname", info.portName());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }

    if (query.next()) {
        query.prepare("UPDATE serialport SET baudrate=:baudrate, databits=:databits, parity=:parity, stopbits=:stopbits, flowcontrol=:flowcontrol, message=:message WHERE portname=:portname");
        query.bindValue(":baudrate", info.baudRate());
        query.bindValue(":databits", info.dataBits());
        query.bindValue(":parity", info.parity());
        query.bindValue(":stopbits", info.stopBits());
        query.bindValue(":flowcontrol", info.flowControl());
        query.bindValue(":message", info.message());
        ok = query.exec();
        if (!ok) {
            qWarning() << query.lastError();
            return QueryError;
        }
    } else {
        query.prepare("INSERT INTO serialport (portname, baudrate, databits, parity, stopbits, flowcontrol, message) VALUES (:portname, :baudrate, :databits, :parity, :stopbits, :flowcontrol, :message)");
        query.bindValue(":portname", info.portName());
        query.bindValue(":baudrate", info.baudRate());
        query.bindValue(":databits", info.dataBits());
        query.bindValue(":parity", info.parity());
        query.bindValue(":stopbits", info.stopBits());
        query.bindValue(":flowcontrol", info.flowControl());
        query.bindValue(":message", info.message());
        ok = query.exec();
        if (!ok) {
            qWarning() << query.lastError();
            return QueryError;
        }
    }

    return NoError;
}

int Database::deleteSerialPort(const SerialPortInfo &info) const
{
    QSqlQuery query;
    bool ok;
    query.prepare("DELETE FROM serialport WHERE portname=:portname");
    query.bindValue(":portname", info.portName());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    return NoError;
}

QList<SerialPortInfo> Database::selectSerialPort() const
{
    QList<SerialPortInfo> infos;

    QSqlQuery query("SELECT * FROM serialport");
    QSqlRecord record = query.record();
    while (query.next()) {
        QString portName = query.value(record.indexOf("portname")).toString();
        int baudRate = query.value(record.indexOf("baudrate")).toInt();
        int dataBits = query.value(record.indexOf("databits")).toInt();
        int parity = query.value(record.indexOf("parity")).toInt();
        int stopBits = query.value(record.indexOf("stopbits")).toInt();
        int flowControl = query.value(record.indexOf("flowcontrol")).toInt();
        QString message = query.value(record.indexOf("message")).toString();

        SerialPortInfo info;
        info.setPortName(portName);
        info.setBaudRate(baudRate);
        info.setDataBits(dataBits);
        info.setParity(parity);
        info.setStopBits(stopBits);
        info.setFlowControl(flowControl);
        info.setMessage(message);
        infos.append(info);
    }
    return infos;
}

int Database::insertRobot(const RobotInfo &info) const
{
    QSqlQuery query;
    bool ok;

    query.prepare("SELECT * FROM robot WHERE name=:name");
    query.bindValue(":name", info.name());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    while (query.next()) {
        int id = query.value(query.record().indexOf("id")).toInt();
        if (id != info.id()) {
            return RobotNameExist;
        }
    }

    query.prepare("SELECT * FROM robot WHERE webhook=:webhook");
    query.bindValue(":webhook", info.webhook());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    while (query.next()) {
        int id = query.value(query.record().indexOf("id")).toInt();
        if (id != info.id()) {
            return RobotWebhookExist;
        }
    }

    query.prepare("INSERT INTO robot (name, webhook, note) VALUES (:name, :webhook, :note)");
    query.bindValue(":name", info.name());
    query.bindValue(":webhook", info.webhook());
    query.bindValue(":note", info.note());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    return NoError;
}

int Database::updateRobot(const RobotInfo &info) const
{
    QSqlQuery query;
    bool ok;

    query.prepare("SELECT * FROM robot WHERE name=:name");
    query.bindValue(":name", info.name());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    while (query.next()) {
        int id = query.value(query.record().indexOf("id")).toInt();
        if (id != info.id()) {
            return RobotNameExist;
        }
    }

    query.prepare("SELECT * FROM robot WHERE webhook=:webhook");
    query.bindValue(":webhook", info.webhook());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    while (query.next()) {
        int id = query.value(query.record().indexOf("id")).toInt();
        if (id != info.id()) {
            return RobotWebhookExist;
        }
    }

    query.prepare("UPDATE robot SET name=:name, webhook=:webhook, note=:note WHERE id=:id");
    query.bindValue(":name", info.name());
    query.bindValue(":webhook", info.webhook());
    query.bindValue(":note", info.note());
    query.bindValue(":id", info.id());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    return NoError;
}

int Database::deleteRobot(const RobotInfo &info) const
{
    QSqlQuery query;
    bool ok;
    query.prepare("DELETE FROM robot WHERE id=:id");
    query.bindValue(":id", info.id());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    return NoError;
}

QList<RobotInfo *> Database::selectRobot() const
{
    QList<RobotInfo *> infos;

    QSqlQuery query("SELECT * FROM robot");
    QSqlRecord record = query.record();
    while (query.next()) {
        int id = query.value(record.indexOf("id")).toInt();
        QString name = query.value(record.indexOf("name")).toString();
        QString webhook = query.value(record.indexOf("webhook")).toString();
        QString note = query.value(record.indexOf("note")).toString();

        RobotInfo *info = new RobotInfo;
        info->setId(id);
        info->setName(name);
        info->setWebhook(webhook);
        info->setNote(note);
        infos.append(info);
    }
    return infos;
}

int Database::updateShortcut(const ShortcutInfo &info) const
{
    QSqlQuery query;
    bool ok;

    query.prepare("REPLACE INTO shortcut (title, commond, color, id) VALUES (:title, :commond, :color, :id)");
    query.bindValue(":title", info.title());
    query.bindValue(":commond", info.commond());
    query.bindValue(":color", info.colorString());
    query.bindValue(":id", info.id());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }

    return NoError;
}

int Database::updateShortcut(const QList<ShortcutInfo> &infoList) const
{
    QElapsedTimer timer;
    timer.start();

    QSqlQuery query;
    bool ok;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        qWarning() << db.lastError();
        return TransactionError;
    }

    foreach (const auto &info, infoList) {
        query.prepare("REPLACE INTO shortcut (title, commond, color, id) VALUES (:title, :commond, :color, :id)");
        query.bindValue(":title", info.title());
        query.bindValue(":commond", info.commond());
        query.bindValue(":color", info.colorString());
        query.bindValue(":id", info.id());
        ok = query.exec();
        if (!ok) {
            qWarning() << query.lastError();
            return QueryError;
        }
    }

    if (!db.commit()) {
        qWarning() << db.lastError();
        return CommitError;
    }

    qDebug() << "took" << timer.elapsed() << "ms";
    return NoError;
}

int Database::deleteShortcut(const ShortcutInfo &info) const
{
    QSqlQuery query;
    bool ok;
    query.prepare("DELETE FROM shortcut WHERE title=:title");
    query.bindValue(":title", info.title());
    ok = query.exec();
    if (!ok) {
        qWarning() << query.lastError();
        return QueryError;
    }
    return NoError;
}

QList<ShortcutInfo> Database::selectShortcut() const
{
    QElapsedTimer timer;
    timer.start();

    QList<ShortcutInfo> infos;

    QSqlQuery query("SELECT title, commond, color, id FROM shortcut");
    while (query.next()) {
        ShortcutInfo info;
        info.setTitle(query.value(0).toString());
        info.setCommond(query.value(1).toString());
        info.setColorString(query.value(2).toString());
        info.setId(query.value(3).toInt());
        infos.append(info);
    }
    qDebug() << "took" << timer.elapsed() << "ms";
    return infos;
}
