#ifndef SERIALPORTSETTING_H
#define SERIALPORTSETTING_H

#include "SerialPortInfo.h"
#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class SerialPortSetting;
}

class SerialPortSetting : public QDialog {
    Q_OBJECT

public:
    explicit SerialPortSetting(QWidget *parent = nullptr);
    ~SerialPortSetting();

    SerialPortInfo serialPortInfo() const;

    QString portName() const;
    QSerialPort::BaudRate baudRate() const;
    QSerialPort::DataBits dataBits() const;
    QSerialPort::Parity parity() const;
    QSerialPort::StopBits stopBits() const;
    QSerialPort::FlowControl flowControl() const;

    QString message() const;

signals:
    void openClicked();

private slots:
    void on_pushButtonOpen_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::SerialPortSetting *ui;
};

#endif // SERIALPORTSETTING_H
