#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H

#include <QDialog>

class RobotTableModel;

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

    void show();

private slots:
    void onTableViewDoubleClicked(int row, int column);

    void on_toolButtonAdd_clicked();
    void on_toolButtonDelete_clicked();

    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    static RobotManager *self;

    Ui::RobotManager *ui;

    RobotTableModel *m_model = nullptr;
};

#endif // ROBOTMANAGER_H
