#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "WSerialPort/WzSerialPort.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslConfiguration>

#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onReadyRead();

    void onNetworkFinished(QNetworkReply *reply);

    void on_pushButton_open_clicked();
    void on_pushButton_clear_receive_clicked();
    void on_pushButton_clear_send_clicked();
    void on_pushButton_send_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *m_timerRead = nullptr;
    WzSerialPort *m_port = nullptr;

    QNetworkAccessManager *m_networkManager = nullptr;
};
#endif // MAINWINDOW_H
