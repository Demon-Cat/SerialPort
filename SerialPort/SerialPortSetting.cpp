#include "SerialPortSetting.h"
#include "ui_SerialPortSetting.h"
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

    ui->comboBoxBaudRate->addItem("1200", QSerialPort::Baud1200);
    ui->comboBoxBaudRate->addItem("2400", QSerialPort::Baud2400);
    ui->comboBoxBaudRate->addItem("4800", QSerialPort::Baud4800);
    ui->comboBoxBaudRate->addItem("9600", QSerialPort::Baud9600);
    ui->comboBoxBaudRate->addItem("19200", QSerialPort::Baud19200);
    ui->comboBoxBaudRate->addItem("38400", QSerialPort::Baud38400);
    ui->comboBoxBaudRate->addItem("57600", QSerialPort::Baud57600);
    ui->comboBoxBaudRate->addItem("115200", QSerialPort::Baud115200);
    ui->comboBoxBaudRate->setCurrentIndex(7);

    ui->comboBoxDatabits->addItem("5", QSerialPort::Data5);
    ui->comboBoxDatabits->addItem("6", QSerialPort::Data6);
    ui->comboBoxDatabits->addItem("7", QSerialPort::Data7);
    ui->comboBoxDatabits->addItem("8", QSerialPort::Data8);
    ui->comboBoxDatabits->setCurrentIndex(3);

    ui->comboBoxParity->addItem("NoParity", QSerialPort::NoParity);
    ui->comboBoxParity->addItem("EvenParity", QSerialPort::EvenParity);
    ui->comboBoxParity->addItem("OddParity", QSerialPort::OddParity);
    ui->comboBoxParity->setCurrentIndex(0);

    ui->comboBoxStopbits->addItem("OneStop", QSerialPort::OneStop);
    ui->comboBoxStopbits->addItem("OneAndHalfStop", QSerialPort::OneAndHalfStop);
    ui->comboBoxStopbits->addItem("TwoStop", QSerialPort::TwoStop);
    ui->comboBoxStopbits->setCurrentIndex(0);

    ui->comboBoxFlowControl->addItem("No", QSerialPort::NoFlowControl);
    ui->comboBoxFlowControl->addItem("RTS/CTS", QSerialPort::HardwareControl);
    ui->comboBoxFlowControl->addItem("XON/XOFF", QSerialPort::SoftwareControl);
    ui->comboBoxFlowControl->setCurrentIndex(0);
}

SerialPortSetting::~SerialPortSetting()
{
    delete ui;
}

SerialPortInfo SerialPortSetting::serialPortInfo() const
{
    SerialPortInfo info;
    info.setPortName(ui->comboBoxPort->currentText());
    info.setBaudRate(ui->comboBoxBaudRate->currentData().toInt());
    info.setDataBits(ui->comboBoxDatabits->currentData().toInt());
    info.setParity(ui->comboBoxParity->currentData().toInt());
    info.setFlowControl(ui->comboBoxFlowControl->currentData().toInt());
    info.setMessage(ui->lineEditMessage->text());
    return info;
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

QString SerialPortSetting::message() const
{
    return ui->lineEditMessage->text();
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
