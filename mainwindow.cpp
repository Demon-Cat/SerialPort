#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QBuffer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_port = new WzSerialPort();

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
    m_port->receive(buf, 1024);

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
        int ok = m_port->open("COM3", 115200, 0, 8, 1);

        if (!ok)
        {
            QMessageBox::warning(this, QString("警告"), QString("打开失败。"));
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
    m_port->send(ba.data(), ba.size());
}

void MainWindow::on_pushButton_clicked()
{
    m_timerRead->start(100);
    return;
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
    QString textMsg = ui->textEdit_send->toPlainText().toUtf8();

    //markdown
    QString markdownMsg = QString(R"({"msgtype":"markdown","markdown":{"content":"设备异常\n>ip:<font color=\"comment\">192.168.1.1</font> \n>机型:<font color=\"comment\">8064</font>"}})");

    request.setUrl(QUrl("https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=79f5d376-a8b8-4038-98c3-45686e84942c"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    m_networkManager->post(request, markdownMsg.toUtf8());
}
