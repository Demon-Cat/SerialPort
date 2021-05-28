#ifndef ADDROBOT_H
#define ADDROBOT_H

#include <QDialog>

namespace Ui {
class AddRobot;
}

class AddRobot : public QDialog
{
    Q_OBJECT

public:
    explicit AddRobot(QWidget *parent = nullptr);
    ~AddRobot();

private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::AddRobot *ui;
};

#endif // ADDROBOT_H
