#include "RobotManager.h"
#include "ui_RobotManager.h"
#include "AddRobot.h"
#include "Database.h"
#include "EditRobot.h"
#include "RobotTableModel.h"
#include <QMessageBox>
#include <QtDebug>

RobotManager *RobotManager::self = nullptr;

RobotManager::RobotManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RobotManager)
{
    ui->setupUi(this);

    m_model = new RobotTableModel(this);
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->tableView, &BaseTableView::doubleClicked, this, &RobotManager::onTableViewDoubleClicked);
}

RobotManager::~RobotManager()
{
    delete ui;
}

void RobotManager::initialize(QWidget *parent)
{
    if (!self) {
        self = new RobotManager(parent);
    }
}

RobotManager *RobotManager::instance()
{
    return self;
}

void RobotManager::show()
{
    QList<RobotInfo *> infos = gDatabase.selectRobot();
    m_model->reset(infos);

    QDialog::show();
}

void RobotManager::onTableViewDoubleClicked(int row, int column)
{
    Q_UNUSED(column)

    RobotInfo *info = static_cast<RobotInfo *>(m_model->info(row));
    EditRobot edit(this);
    int result = edit.execEdit(info);
    if (result == EditRobot::Accepted) {
        QList<RobotInfo *> infos = gDatabase.selectRobot();
        m_model->reset(infos);
    }
}

void RobotManager::on_toolButtonAdd_clicked()
{
    AddRobot add(this);
    int result = add.exec();
    if (result == AddRobot::Accepted) {
        QList<RobotInfo *> infos = gDatabase.selectRobot();
        m_model->reset(infos);
    }
}

void RobotManager::on_toolButtonDelete_clicked()
{
    int row = ui->tableView->selectedRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择需要删除的项！");
        return;
    }
    RobotInfo *info = static_cast<RobotInfo *>(m_model->info(row));
    int result = QMessageBox::question(this, "提示", QString("确定要删除“%1”吗？").arg(info->name()));
    if (result == QMessageBox::Yes) {
        gDatabase.deleteRobot(*info);
        m_model->removeRow(row);
    }
}

void RobotManager::on_pushButtonOk_clicked()
{
    accept();
}

void RobotManager::on_pushButtonCancel_clicked()
{
    reject();
}
