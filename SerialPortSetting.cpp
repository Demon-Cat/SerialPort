#include "SerialPortSetting.h"
#include "ui_SerialPortSetting.h"
#include "Settings.h"
#include <QMap>

SerialPortSetting::SerialPortSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPortSetting)
{
    ui->setupUi(this);

    QMap<int, QString> names;
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    for (int i = 0; i < infoList.size(); ++i) {
        QString name = infoList.at(i).portName();
        int value = name.midRef(3).toInt();
        names.insert(value, name);
    }
    for (auto iter = names.constBegin(); iter != names.constEnd(); ++iter) {
        ui->comboBoxPort->addItem(iter.value());
    }
    ui->comboBoxPort->setCurrentIndex(ui->comboBoxPort->findText(gSettings->portName()));

    ui->comboBoxBaudRate->addItem("1200", QSerialPort::Baud1200);
    ui->comboBoxBaudRate->addItem("2400", QSerialPort::Baud2400);
    ui->comboBoxBaudRate->addItem("4800", QSerialPort::Baud4800);
    ui->comboBoxBaudRate->addItem("9600", QSerialPort::Baud9600);
    ui->comboBoxBaudRate->addItem("19200", QSerialPort::Baud19200);
    ui->comboBoxBaudRate->addItem("38400", QSerialPort::Baud38400);
    ui->comboBoxBaudRate->addItem("57600", QSerialPort::Baud57600);
    ui->comboBoxBaudRate->addItem("115200", QSerialPort::Baud115200);
    int index = ui->comboBoxBaudRate->findData(gSettings->baudRate());
    if (index < 0) {
        ui->comboBoxBaudRate->setCurrentIndex(7);
    } else {
        ui->comboBoxBaudRate->setCurrentIndex(index);
    }

    ui->comboBoxDatabits->addItem("5", QSerialPort::Data5);
    ui->comboBoxDatabits->addItem("6", QSerialPort::Data6);
    ui->comboBoxDatabits->addItem("7", QSerialPort::Data7);
    ui->comboBoxDatabits->addItem("8", QSerialPort::Data8);
    index = ui->comboBoxDatabits->findData(gSettings->dataBits());
    if (index < 0) {
        ui->comboBoxDatabits->setCurrentIndex(3);
    } else {
        ui->comboBoxDatabits->setCurrentIndex(index);
    }

    ui->comboBoxParity->addItem("NoParity", QSerialPort::NoParity);
    ui->comboBoxParity->addItem("EvenParity", QSerialPort::EvenParity);
    ui->comboBoxParity->addItem("OddParity", QSerialPort::OddParity);
    index = ui->comboBoxParity->findData(gSettings->parity());
    if (index < 0) {
        ui->comboBoxParity->setCurrentIndex(0);
    } else {
        ui->comboBoxParity->setCurrentIndex(index);
    }

    ui->comboBoxStopbits->addItem("OneStop", QSerialPort::OneStop);
    ui->comboBoxStopbits->addItem("OneAndHalfStop", QSerialPort::OneAndHalfStop);
    ui->comboBoxStopbits->addItem("TwoStop", QSerialPort::TwoStop);
    ui->comboBoxStopbits->setCurrentIndex(0);
    index = ui->comboBoxStopbits->findData(gSettings->stopBits());
    if (index < 0) {
        ui->comboBoxStopbits->setCurrentIndex(0);
    } else {
        ui->comboBoxStopbits->setCurrentIndex(index);
    }

    ui->comboBoxFlowControl->addItem("No", QSerialPort::NoFlowControl);
    ui->comboBoxFlowControl->addItem("RTS/CTS", QSerialPort::HardwareControl);
    ui->comboBoxFlowControl->addItem("XON/XOFF", QSerialPort::SoftwareControl);
    index = ui->comboBoxFlowControl->findData(gSettings->flowControl());
    if (index < 0) {
        ui->comboBoxFlowControl->setCurrentIndex(0);
    } else {
        ui->comboBoxFlowControl->setCurrentIndex(index);
    }
}

SerialPortSetting::~SerialPortSetting()
{
    delete ui;
}

QString SerialPortSetting::portName() const
{
    return ui->comboBoxPort->currentText();
}

QSerialPort::BaudRate SerialPortSetting::baudRate() const
{
    return ui->comboBoxBaudRate->currentData().value<QSerialPort::BaudRate>();
}

QSerialPort::DataBits SerialPortSetting::dataBits() const
{
    return ui->comboBoxDatabits->currentData().value<QSerialPort::DataBits>();
}

QSerialPort::Parity SerialPortSetting::parity() const
{
    return ui->comboBoxParity->currentData().value<QSerialPort::Parity>();
}

QSerialPort::StopBits SerialPortSetting::stopBits() const
{
    return ui->comboBoxStopbits->currentData().value<QSerialPort::StopBits>();
}

QSerialPort::FlowControl SerialPortSetting::flowControl() const
{
    return ui->comboBoxFlowControl->currentData().value<QSerialPort::FlowControl>();
}

void SerialPortSetting::on_pushButtonOpen_clicked()
{
    accept();
    emit openClicked();
}

void SerialPortSetting::on_pushButtonCancel_clicked()
{
    reject();
}
