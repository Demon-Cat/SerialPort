#include "SerialPortInfo.h"

SerialPortInfo::SerialPortInfo()
{

}

const QString &SerialPortInfo::portName() const
{
    return m_portName;
}

void SerialPortInfo::setPortName(const QString &newPortName)
{
    m_portName = newPortName;
}

int SerialPortInfo::baudRate() const
{
    return m_baudRate;
}

void SerialPortInfo::setBaudRate(int newBaudRate)
{
    m_baudRate = newBaudRate;
}

int SerialPortInfo::dataBits() const
{
    return m_dataBits;
}

void SerialPortInfo::setDataBits(int newDataBits)
{
    m_dataBits = newDataBits;
}

int SerialPortInfo::parity() const
{
    return m_parity;
}

void SerialPortInfo::setParity(int newParity)
{
    m_parity = newParity;
}

int SerialPortInfo::stopBits() const
{
    return m_stopBits;
}

void SerialPortInfo::setStopBits(int newStopBits)
{
    m_stopBits = newStopBits;
}

int SerialPortInfo::flowControl() const
{
    return m_flowControl;
}

void SerialPortInfo::setFlowControl(int newFlowControl)
{
    m_flowControl = newFlowControl;
}

const QString &SerialPortInfo::message() const
{
    return m_message;
}

void SerialPortInfo::setMessage(const QString &newMessage)
{
    m_message = newMessage;
}
