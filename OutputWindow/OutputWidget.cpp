#include "OutputWidget.h"
#include "ui_OutputWidget.h"

OutputWidget::OutputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputWidget)
{
    ui->setupUi(this);


    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &OutputWidget::onReadyRead);

    connect(ui->plainTextEditReceive, &OutputTextEdit::input, this, &OutputWidget::onOutputTextEditInput);
}

OutputWidget::~OutputWidget()
{
    delete ui;
}

void OutputWidget::setSerialPortInfo(const SerialPortInfo &info)
{
    m_info = info;

    m_serialPort->setPortName(m_info.portName());
    m_serialPort->setBaudRate(m_info.baudRate());
    m_serialPort->setDataBits((QSerialPort::DataBits)m_info.dataBits());
    m_serialPort->setParity((QSerialPort::Parity)m_info.parity());
    m_serialPort->setStopBits((QSerialPort::StopBits)m_info.stopBits());
    m_serialPort->setFlowControl((QSerialPort::FlowControl)m_info.flowControl());
}

SerialPortInfo OutputWidget::serialPortInfo() const
{
    return m_info;
}

QString OutputWidget::portName() const
{
    return m_info.portName();
}

QString OutputWidget::message() const
{
    return m_info.message();
}

QString OutputWidget::stateString() const
{
    QString text;
    text.append("串口: ");
    text.append(portName());
    if (!message().isEmpty()) {
        text.append(QString(", %1").arg(message()));
    }
    if (isOpen()) {
        text.append(", 已打开");
    } else {
        text.append(", 未打开");
        if (m_serialPort->error() != QSerialPort::NoError) {
            text.append(QString(", %1").arg(errorString()));
        }
    }
    return text;
}

QString OutputWidget::errorString() const
{
    return m_serialPort->errorString();
}

bool OutputWidget::openSerialPort()
{
    if (!m_serialPort->open(QSerialPort::ReadWrite)) {
        return false;
    } else {
        return true;
    }
}

void OutputWidget::closeSerialPort()
{
    m_serialPort->close();
}

bool OutputWidget::isOpen() const
{
    return m_serialPort->isOpen();
}

quint64 OutputWidget::writeSerialPort(const QByteArray &data)
{
    if (!m_serialPort->isOpen()) {
        return 0;
    }

    return m_serialPort->write(data);
}

/**
 * @brief OutputWidget::onReadyRead
 * "\x1b[J" 清除光标到最后的所有字符
 * "\x1b[K" 清除光标到当前行最后的所有字符
 * "\a" 响铃
 * "\b" 退格
 */
void OutputWidget::onReadyRead()
{
    //没考虑粘包
    QByteArray buff = m_serialPort->readAll();

    QByteArray data;
    for (int i = 0; i < buff.size(); ++i) {
        const char &c = buff.at(i);
        switch (c) {
        case '\a':
            ui->plainTextEditReceive->appendMessage(data);
            data.clear();
            break;
        case '\b':
            ui->plainTextEditReceive->appendMessage(data);
            data.clear();
            ui->plainTextEditReceive->movePosition(QTextCursor::Left);
            break;
        case '\x1b':
            if (i + 2 < buff.size() && buff.at(i + 1) == '[' && buff.at(i + 2) == 'J') {
                ui->plainTextEditReceive->appendMessage(data);
                data.clear();
                ui->plainTextEditReceive->deleteAfterChars();
                i += 2;
            } else {
                data.append(c);
            }
            break;
        case '\r':
            if (i + 1 < buff.size() && buff.at(i + 1) != '\n') {
                ui->plainTextEditReceive->appendMessage(data);
                data.clear();
                ui->plainTextEditReceive->movePosition(QTextCursor::StartOfBlock);
            } else {
                data.append(c);
            }
            break;
        default:
            data.append(c);
            break;
        }
    }
    ui->plainTextEditReceive->appendMessage(data);
}

void OutputWidget::onOutputTextEditInput(const QByteArray &data)
{
    writeSerialPort(data);
}
