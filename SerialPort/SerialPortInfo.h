#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H

#include <QString>

class SerialPortInfo
{
public:
    SerialPortInfo();

    const QString &portName() const;
    void setPortName(const QString &newPortName);

    int baudRate() const;
    void setBaudRate(int newBaudRate);

    int dataBits() const;
    void setDataBits(int newDataBits);

    int parity() const;
    void setParity(int newParity);

    int stopBits() const;
    void setStopBits(int newStopBits);

    int flowControl() const;
    void setFlowControl(int newFlowControl);

    const QString &message() const;
    void setMessage(const QString &newMessage);

private:
    QString m_portName;
    int m_baudRate = 115200;
    int m_dataBits = 8;
    int m_parity = 0;
    int m_stopBits = 1;
    int m_flowControl = 0;

    QString m_message;
};

#endif // SERIALPORTINFO_H
