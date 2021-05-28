#ifndef EDITROBOT_H
#define EDITROBOT_H

#include <QDialog>

class RobotInfo;

namespace Ui {
class EditRobot;
}

class EditRobot : public QDialog
{
    Q_OBJECT

public:
    explicit EditRobot(QWidget *parent = nullptr);
    ~EditRobot();

    int execEdit(RobotInfo *info);

private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::EditRobot *ui;

    RobotInfo *m_info = nullptr;
};

#endif // EDITROBOT_H
