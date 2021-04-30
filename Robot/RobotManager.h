#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H

#include <QDialog>

#define gRobotManager RobotManager::instance()

namespace Ui {
class RobotManager;
}

class RobotManager : public QDialog
{
    Q_OBJECT

public:
    explicit RobotManager(QWidget *parent = nullptr);
    ~RobotManager();

    static void initialize(QWidget *parent);
    static RobotManager *instance();

private:
    static RobotManager *self;

    Ui::RobotManager *ui;
};

#endif // ROBOTMANAGER_H
