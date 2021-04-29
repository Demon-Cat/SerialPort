#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SerialPortSetting.h"
#include "Settings.h"
#include <QBuffer>
#include <QMessageBox>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_serialPortSetting = new SerialPortSetting(this);
    connect(m_serialPortSetting, &SerialPortSetting::openClicked, this, &MainWindow::onSerialPortOpenClicked);

    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &MainWindow::onSerialPortErrorOccurred);
    connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);

    connect(ui->plainTextEditReceive, &OutputTextEdit::getData, this, &MainWindow::onOutputTextEditInput);
    connect(ui->plainTextEditSend, &InputTextEdit::enterPressed, this, &MainWindow::onInputTextEditEnterPressed);

    QFont font("Source Code Pro", 9);
    ui->plainTextEditReceive->setFont(font);
    ui->plainTextEditSend->setFont(font);

    ui->splitter->restoreState(gSettings->splitterState());

    //
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkFinished);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);

    if (gSettings->isMaximized()) {
        showMaximized();
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    delete Settings::instance();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::No == QMessageBox::question(this, "提示", "确定要退出吗？")) {
        event->ignore();
        return;
    }

    gSettings->saveSplitterState(ui->splitter->saveState());
    gSettings->saveMaximized(isMaximized());
}

quint64 MainWindow::writeSerialPort(const QByteArray &data)
{
    if (!m_serialPort->isOpen()) {
        return 0;
    }

    return m_serialPort->write(data);
}

void MainWindow::onSerialPortErrorOccurred(QSerialPort::SerialPortError error)
{
    qDebug() << error;
}

void MainWindow::onReadyRead()
{
    QByteArray data = m_serialPort->readAll();
    if (data.isEmpty()) {
        return;
    }

    if (data.startsWith("\a")) {
        return;
    }

    //退格
    if (data.startsWith("\b")) {
        //左移动光标
        if (data.size() == 1) {
            ui->plainTextEditReceive->movePosition(QTextCursor::Left);
            return;
        }
        //退格，光标在最后，如果收到081B5B4A，则执行退格操作
        if (data.at(1) == 0x1b) {
            ui->plainTextEditReceive->deletePreviousChar();
            return;
        }
        //退格，光标不在最后，删除光标位置和之后的所有字符，串口会发送回来多删除的字符
        if (data.at(1) != 0x1b) {
            ui->plainTextEditReceive->deleteAfterChars();
            int end = data.indexOf(0x1b);
            QByteArray ba = data.mid(1, end - 1);
            ui->plainTextEditReceive->insertMessage(ba, true);
            return;
        }
    }

    //右移动光标，会收到字符，从当前光标位置覆盖
    if (data.size() == 1) {
        ui->plainTextEditReceive->overwriteMessage(data);
        return;
    }

    //
    ui->plainTextEditReceive->appendMessage(data);
}

void MainWindow::onOutputTextEditInput(const QByteArray &data)
{
    writeSerialPort(data);
}

void MainWindow::onInputTextEditEnterPressed()
{
    QByteArray ba = ui->plainTextEditSend->toPlainText().toUtf8();
    ba.append("\r");
    writeSerialPort(ba);

    ui->plainTextEditSend->clear();
}

void MainWindow::onNetworkFinished(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
    reply->deleteLater();
}

void MainWindow::onSerialPortOpenClicked()
{
    m_serialPort->setPortName(m_serialPortSetting->portName());
    m_serialPort->setBaudRate(m_serialPortSetting->baudRate());
    m_serialPort->setDataBits(m_serialPortSetting->dataBits());
    m_serialPort->setParity(m_serialPortSetting->parity());
    m_serialPort->setStopBits(m_serialPortSetting->stopBits());
    m_serialPort->setFlowControl(m_serialPortSetting->flowControl());

    gSettings->savePortName(m_serialPort->portName());
    gSettings->saveBaudRate(m_serialPort->baudRate());
    gSettings->saveDataBits(m_serialPort->dataBits());
    gSettings->saveParity(m_serialPort->parity());
    gSettings->saveStopBits(m_serialPort->stopBits());
    gSettings->saveFlowControl(m_serialPort->flowControl());

    if (!m_serialPort->open(QSerialPort::ReadWrite)) {
        QMessageBox::warning(this, QString("警告"), QString("打开失败：%1").arg(m_serialPort->errorString()));
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->labelSerialPortStatus->setText(QString("未打开"));
    } else {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->labelSerialPortStatus->setText(QString("串口：%1，%2").arg(m_serialPort->portName()).arg(m_serialPort->baudRate()));
    }
}

#if 0
void MainWindow::on_pushButton_clicked()
{
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_1);

    QNetworkRequest request;
    request.setSslConfiguration(config);

    //image
    QImage image("F:/Qt/SerialPort/bin/0.png");
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");
    QString base64 = imageData.toBase64();
    QString md5 = QCryptographicHash::hash(imageData, QCryptographicHash::Md5).toHex();
    QString imageMsg = QString(R"({"msgtype":"image","image":{"base64":"%1","md5":"%2"}})").arg(base64).arg(md5);

    //text
    QString textMsg = ui->plainTextEditSend->toPlainText().toUtf8();

    //markdown
    QString markdownMsg = QString(R"({"msgtype":"markdown","markdown":{"content":"设备异常\n>ip:<font color=\"comment\">192.168.1.1</font> \n>机型:<font color=\"comment\">8064</font>"}})");

    request.setUrl(QUrl("https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=79f5d376-a8b8-4038-98c3-45686e84942c"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    m_networkManager->post(request, markdownMsg.toUtf8());
}
#endif

void MainWindow::on_actionConnect_triggered()
{
    m_serialPortSetting->open();
}

void MainWindow::on_actionDisconnect_triggered()
{
    m_serialPort->close();

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->labelSerialPortStatus->setText(QString("串口：未打开"));
}
