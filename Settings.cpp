#include "Settings.h"
#include <QGuiApplication>

Settings *Settings::self = nullptr;

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    QString filePath = QString("%1/settings.ini").arg(qApp->applicationDirPath());
    m_settings = new QSettings(filePath, QSettings::IniFormat, this);
}

Settings *Settings::instance()
{
    if (!self) {
        self = new Settings();
    }
    return self;
}

void Settings::saveSplitterState(const QByteArray &value)
{
    m_settings->setValue("Splitter/State", value);
}

QByteArray Settings::splitterState() const
{
    return m_settings->value("Splitter/State").toByteArray();
}

void Settings::saveMaximized(bool value)
{
    m_settings->setValue("MainWindow/Maximized", value);
}

bool Settings::isMaximized() const
{
    return m_settings->value("MainWindow/Maximized").toBool();
}

void Settings::savePortName(const QString &value)
{
    m_settings->setValue("SerialPort/Name", value);
}

QString Settings::portName() const
{
    return m_settings->value("SerialPort/Name").toString();
}

void Settings::saveBaudRate(int value)
{
    m_settings->setValue("SerialPort/BaudRate", value);
}

int Settings::baudRate() const
{
    return m_settings->value("SerialPort/BaudRate").toInt();
}

void Settings::saveDataBits(int value)
{
    m_settings->setValue("SerialPort/DataBits", value);
}

int Settings::dataBits() const
{
    return m_settings->value("SerialPort/DataBits").toInt();
}

void Settings::saveParity(int value)
{
    m_settings->setValue("SerialPort/Parity", value);
}

int Settings::parity() const
{
    return m_settings->value("SerialPort/Parity").toInt();
}

void Settings::saveStopBits(int value)
{
    m_settings->setValue("SerialPort/StopBits", value);
}

int Settings::stopBits() const
{
    return m_settings->value("SerialPort/StopBits").toInt();
}

void Settings::saveFlowControl(int value)
{
    m_settings->setValue("SerialPort/FlowControl", value);
}

int Settings::flowControl() const
{
    return m_settings->value("SerialPort/FlowControl").toInt();
}
