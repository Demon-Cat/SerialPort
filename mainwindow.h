#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>

class SerialPortSetting;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    quint64 writeSerialPort(const QByteArray &data);

private slots:
    void onSerialPortOpenClicked();
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError error);
    void onReadyRead();
    void onOutputTextEditInput(const QByteArray &data);
    void onInputTextEditEnterPressed();

    void onNetworkFinished(QNetworkReply *reply);

    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionRobot_triggered();
    void on_actionKeywordMonitor_triggered();

private:
    Ui::MainWindow *ui;

    SerialPortSetting *m_serialPortSetting = nullptr;
    QSerialPort *m_serialPort = nullptr;

    QNetworkAccessManager *m_networkManager = nullptr;
};
#endif // MAINWINDOW_H
