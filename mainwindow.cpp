#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Database.h"
#include "KeywordMonitor.h"
#include "OutputWidget.h"
#include "RobotManager.h"
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

    KeywordMonitor::initialize(this);
    RobotManager::initialize(this);

    connect(ui->tabBar, &TabBar::addClicked, this, &MainWindow::onTabAddClicked);
    connect(ui->tabBar, &TabBar::tabClicked, this, &MainWindow::onTabClicked);
    connect(ui->tabBar, &TabBar::tabCloseRequested, this, &MainWindow::onTabCloseRequested);

    connect(ui->shortcutWidget, &ShortcutWidget::input, this, &MainWindow::onShortcutInput);
    connect(ui->plainTextEditSend, &InputTextEdit::enterPressed, this, &MainWindow::onInputTextEditEnterPressed);

    ui->splitter->restoreState(gSettings.splitterState());

    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkFinished);

    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(false);

    if (gSettings.isMaximized()) {
        showMaximized();
    }

    QList<SerialPortInfo> listSerialPortInfo = gDatabase.selectSerialPort();
    if (!listSerialPortInfo.isEmpty()) {
        foreach (const SerialPortInfo &info, listSerialPortInfo) {
            addTab(info);
        }
        ui->tabBar->setCurrentTab(gSettings.portName());
    }

    qDebug() << "SSL library in use at compile time:" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "SSL library in use at run-time:" << QSslSocket::sslLibraryVersionString();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::No == QMessageBox::question(this, "提示", "确定要退出吗？")) {
        event->ignore();
        return;
    }

    gSettings.saveSplitterState(ui->splitter->saveState());
    gSettings.saveMaximized(isMaximized());
}

void MainWindow::addTab(const SerialPortInfo &info)
{
    ui->tabBar->addTab(info.portName());

    OutputWidget *widget = new OutputWidget(this);
    widget->setSerialPortInfo(info);
    if (widget->openSerialPort()) {
        ui->tabBar->setTabState(widget->portName(), TabButton::StateConnected);
    } else {
        ui->tabBar->setTabState(widget->portName(), TabButton::StateDisconnected);
    }

    m_mapOutputWidget.insert(info.portName(), widget);
}

OutputWidget *MainWindow::currentOutputWidget()
{
    QString text = ui->tabBar->currentTab();
    return m_mapOutputWidget.value(text);
}

void MainWindow::onTabAddClicked()
{
    SerialPortSetting setting(this);
    int result = setting.exec();
    if (result == SerialPortSetting::Accepted) {
        const SerialPortInfo &info = setting.serialPortInfo();
        addTab(info);
        ui->tabBar->setCurrentTab(info.portName());

        gDatabase.updateSerialPort(info);
    }
}

void MainWindow::onTabClicked(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    QLayoutItem *item = ui->outputLayout->itemAtPosition(0, 0);
    if (item) {
        QWidget *old = item->widget();
        if (old) {
            ui->outputLayout->removeWidget(old);
            old->hide();
        }
    }

    OutputWidget *widget = m_mapOutputWidget.value(text);
    if (widget) {
        ui->outputLayout->addWidget(widget, 0, 0);
        widget->show();

        if (widget->isOpen()) {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->labelSerialPortStatus->setText(widget->stateString());
            ui->tabBar->setTabState(widget->portName(), TabButton::StateConnected);
        } else {
            ui->actionConnect->setEnabled(true);
            ui->actionDisconnect->setEnabled(false);
            ui->labelSerialPortStatus->setText(widget->stateString());
            ui->tabBar->setTabState(widget->portName(), TabButton::StateDisconnected);
        }

        gSettings.savePortName(text);
    }
}

void MainWindow::onTabCloseRequested(const QString &text)
{
    OutputWidget *widget = m_mapOutputWidget.value(text);
    if (widget) {
        gDatabase.deleteSerialPort(widget->serialPortInfo());
        widget->closeSerialPort();
        widget->deleteLater();
        m_mapOutputWidget.remove(text);
        ui->tabBar->removeTab(text);
    }
}

void MainWindow::onShortcutInput(const QByteArray &data)
{
    OutputWidget *output = currentOutputWidget();
    if (output) {
        output->writeSerialPort(data);
    }
}

void MainWindow::onInputTextEditEnterPressed()
{
    OutputWidget *output = currentOutputWidget();
    if (output) {
        QByteArray ba = ui->plainTextEditSend->toPlainText().toUtf8();
        ba.append("\r");
        output->writeSerialPort(ba);
    }

    ui->plainTextEditSend->clear();
}

void MainWindow::onNetworkFinished(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
    reply->deleteLater();
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
    OutputWidget *widget = currentOutputWidget();
    if (widget) {
        if (widget->openSerialPort()) {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->labelSerialPortStatus->setText(widget->stateString());
            ui->tabBar->setTabState(widget->portName(), TabButton::StateConnected);
        } else {
            ui->actionConnect->setEnabled(true);
            ui->actionDisconnect->setEnabled(false);
            ui->labelSerialPortStatus->setText(widget->stateString());
            ui->tabBar->setTabState(widget->portName(), TabButton::StateDisconnected);
        }
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    OutputWidget *widget = currentOutputWidget();
    if (widget) {
        widget->closeSerialPort();

        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->labelSerialPortStatus->setText(widget->stateString());
        ui->tabBar->setTabState(widget->portName(), TabButton::StateDisconnected);
    }
}

void MainWindow::on_actionRobot_triggered()
{
    gRobotManager->show();
}

void MainWindow::on_actionKeywordMonitor_triggered()
{
    gKeyworkMonitor->show();
}
