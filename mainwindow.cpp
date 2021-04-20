#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    for (int i = 0; i < infoList.size(); ++i)
    {
        ui->comboBox_port->addItem(infoList.at(i).portName());
    }

    ui->comboBox_baudrate->addItem("1200", QSerialPort::Baud1200);
    ui->comboBox_baudrate->addItem("2400", QSerialPort::Baud2400);
    ui->comboBox_baudrate->addItem("4800", QSerialPort::Baud4800);
    ui->comboBox_baudrate->addItem("9600", QSerialPort::Baud9600);
    ui->comboBox_baudrate->addItem("19200", QSerialPort::Baud19200);
    ui->comboBox_baudrate->addItem("38400", QSerialPort::Baud38400);
    ui->comboBox_baudrate->addItem("57600", QSerialPort::Baud57600);
    ui->comboBox_baudrate->addItem("115200", QSerialPort::Baud115200);
    ui->comboBox_baudrate->setCurrentIndex(3);

    ui->comboBox_databits->addItem("5", QSerialPort::Data5);
    ui->comboBox_databits->addItem("6", QSerialPort::Data6);
    ui->comboBox_databits->addItem("7", QSerialPort::Data7);
    ui->comboBox_databits->addItem("8", QSerialPort::Data8);
    ui->comboBox_databits->setCurrentIndex(3);

    ui->comboBox_parity->addItem("NoParity", QSerialPort::NoParity);
    ui->comboBox_parity->addItem("EvenParity", QSerialPort::EvenParity);
    ui->comboBox_parity->addItem("OddParity", QSerialPort::OddParity);
    ui->comboBox_parity->setCurrentIndex(0);

    ui->comboBox_stopbits->addItem("OneStop", QSerialPort::OneStop);
    ui->comboBox_stopbits->addItem("OneAndHalfStop", QSerialPort::OneAndHalfStop);
    ui->comboBox_stopbits->addItem("TwoStop", QSerialPort::TwoStop);
    ui->comboBox_stopbits->setCurrentIndex(0);

    m_port = new QSerialPort(this);
    connect(m_port, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
    QByteArray ba = m_port->readAll();

    if (ui->checkBox_receive_hex->isChecked())
    {
        ui->textEdit_receive->append(ba.toHex().toUpper());
    }
    else
    {
        ui->textEdit_receive->append(ba);
    }
}


void MainWindow::on_pushButton_open_clicked()
{
    if (ui->pushButton_open->text() == "打开")
    {
        m_port->setPortName(ui->comboBox_port->currentText());
        m_port->setBaudRate(ui->comboBox_baudrate->currentData().value<QSerialPort::BaudRate>());
        m_port->setDataBits(ui->comboBox_databits->currentData().value<QSerialPort::DataBits>());
        m_port->setParity(ui->comboBox_parity->currentData().value<QSerialPort::Parity>());
        m_port->setStopBits(ui->comboBox_stopbits->currentData().value<QSerialPort::StopBits>());
        m_port->setFlowControl(QSerialPort::HardwareControl);

        if (!m_port->open(QSerialPort::ReadWrite))
        {
            QMessageBox::warning(this, QString("警告"), QString("打开失败：%1").arg(m_port->errorString()));
            return;
        }

        ui->comboBox_port->setEnabled(false);
        ui->comboBox_baudrate->setEnabled(false);
        ui->comboBox_databits->setEnabled(false);
        ui->comboBox_parity->setEnabled(false);
        ui->comboBox_stopbits->setEnabled(false);
        ui->pushButton_open->setText("关闭");
    }
    else
    {
        m_port->close();

        ui->comboBox_port->setEnabled(true);
        ui->comboBox_baudrate->setEnabled(true);
        ui->comboBox_databits->setEnabled(true);
        ui->comboBox_parity->setEnabled(true);
        ui->comboBox_stopbits->setEnabled(true);
        ui->pushButton_open->setText("打开");
    }
}

void MainWindow::on_pushButton_clear_receive_clicked()
{
    ui->textEdit_receive->clear();
}

void MainWindow::on_pushButton_clear_send_clicked()
{
    ui->textEdit_send->clear();
}

void MainWindow::on_pushButton_send_clicked()
{
    if (ui->checkBox_send_hex->isChecked())
    {
        QByteArray ba = QByteArray::fromHex(ui->textEdit_send->toPlainText().toLatin1());
        m_port->write(ba);
    }
    else
    {
        QByteArray ba = ui->textEdit_send->toPlainText().toUtf8();
        ba.append("\r");
        m_port->write(ba);
    }
}
