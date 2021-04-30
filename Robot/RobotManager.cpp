#include "RobotManager.h"
#include "ui_RobotManager.h"

RobotManager *RobotManager::self = nullptr;

RobotManager::RobotManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RobotManager)
{
    ui->setupUi(this);
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
