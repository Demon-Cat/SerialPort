#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include "SerialPortInfo.h"

namespace Ui {
class OutputWidget;
}

class OutputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWidget(QWidget *parent = nullptr);
    ~OutputWidget();

    void setSerialPortInfo(const SerialPortInfo &info);
    SerialPortInfo serialPortInfo() const;

    QString portName() const;
    QString message() const;

    QString stateString() const;
    QString errorString() const;

    bool openSerialPort();
    void closeSerialPort();
    bool isOpen() const;

    quint64 writeSerialPort(const QByteArray &data);

private:

private slots:
    void onReadyRead();
    void onOutputTextEditInput(const QByteArray &data);

private:
    Ui::OutputWidget *ui;

    SerialPortInfo m_info;
    QSerialPort *m_serialPort = nullptr;
};

#endif // OUTPUTWIDGET_H
