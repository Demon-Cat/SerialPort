#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>

#include "SerialPortInfo.h"

class OutputWidget;

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
    void addTab(const SerialPortInfo &info);
    OutputWidget *currentOutputWidget();

private slots:
    void onTabAddClicked();
    void onTabClicked(const QString &text);
    void onTabCloseRequested(const QString &text);

    void onShortcutInput(const QByteArray &data);
    void onInputTextEditEnterPressed();

    void onNetworkFinished(QNetworkReply *reply);

    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionRobot_triggered();
    void on_actionKeywordMonitor_triggered();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *m_networkManager = nullptr;

    QMap<QString, OutputWidget *> m_mapOutputWidget;
};
#endif // MAINWINDOW_H
