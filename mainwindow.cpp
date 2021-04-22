#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    QList<QextPortInfo> infos = QextSerialEnumerator::getPorts();
    for (int i = 0; i < infos.size(); ++i)
    {
        const QextPortInfo &info = infos.at(i);
        ui->comboBox_port->addItem(info.portName);
    }

    ui->comboBox_baudrate->addItem("1200", 1200);
    ui->comboBox_baudrate->addItem("2400", 2400);
    ui->comboBox_baudrate->addItem("4800", 4800);
    ui->comboBox_baudrate->addItem("9600", 9600);
    ui->comboBox_baudrate->addItem("19200", 19200);
    ui->comboBox_baudrate->addItem("38400", 38400);
    ui->comboBox_baudrate->addItem("57600", 57600);
    ui->comboBox_baudrate->addItem("115200", 115200);
    ui->comboBox_baudrate->setCurrentIndex(3);

    ui->comboBox_databits->addItem("5", 5);
    ui->comboBox_databits->addItem("6", 6);
    ui->comboBox_databits->addItem("7", 7);
    ui->comboBox_databits->addItem("8", 8);
    ui->comboBox_databits->setCurrentIndex(3);

    ui->comboBox_parity->addItem("None", PAR_NONE);
    ui->comboBox_parity->addItem("Odd", PAR_ODD);
    ui->comboBox_parity->addItem("Even", PAR_EVEN);
    ui->comboBox_parity->addItem("Mark", PAR_MARK);
    ui->comboBox_parity->addItem("Space", PAR_SPACE);
    ui->comboBox_parity->setCurrentIndex(0);

    ui->comboBox_stopbits->addItem("1", STOP_1);
    ui->comboBox_stopbits->addItem("1.5", STOP_1_5);
    ui->comboBox_stopbits->addItem("2", STOP_2);
    ui->comboBox_stopbits->setCurrentIndex(0);

    m_port = new QextSerialPort(QextSerialPort::EventDriven, this);
    connect(m_port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
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
        m_port->setBaudRate(BaudRateType(ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt()));
        m_port->setDataBits(DataBitsType(ui->comboBox_databits->itemData(ui->comboBox_databits->currentIndex()).toInt()));
        m_port->setParity(ParityType(ui->comboBox_parity->itemData(ui->comboBox_parity->currentIndex()).toInt()));
        m_port->setStopBits(StopBitsType(ui->comboBox_stopbits->itemData(ui->comboBox_stopbits->currentIndex()).toInt()));

        if (!m_port->open(QextSerialPort::ReadWrite))
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
