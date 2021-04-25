#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QBuffer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const char *portName = "\\\\.\\COM3";
    m_port = new SerialPort(portName);

    m_timerRead = new QTimer(this);
    connect(m_timerRead, SIGNAL(timeout()), this, SLOT(onReadyRead()));

    //
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    m_port->readSerialPort(buf, 1024);

    ui->textEdit_receive->append(QString(buf));
}

void MainWindow::onNetworkFinished(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
    reply->deleteLater();
}

void MainWindow::on_pushButton_open_clicked()
{
    if (ui->pushButton_open->text() == "打开")
    {

        ui->comboBox_port->setEnabled(false);
        ui->comboBox_baudrate->setEnabled(false);
        ui->comboBox_databits->setEnabled(false);
        ui->comboBox_parity->setEnabled(false);
        ui->comboBox_stopbits->setEnabled(false);
        ui->pushButton_open->setText("关闭");
    }
    else
    {

        ui->comboBox_port->setEnabled(true);
        ui->comboBox_baudrate->setEnabled(true);
        ui->comboBox_databits->setEnabled(true);
        ui->comboBox_parity->setEnabled(true);
        ui->comboBox_stopbits->setEnabled(true);
        ui->pushButton_open->setText("打开");

        m_timerRead->stop();
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
    QByteArray ba = ui->textEdit_send->toPlainText().toUtf8();
    ba.append("\r");
    //m_port->send(ba.data(), ba.size());
}

void MainWindow::on_pushButton_clicked()
{
    onReadyRead();
}
