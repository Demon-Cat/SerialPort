#include "KeywordMonitor.h"
#include "ui_KeywordMonitor.h"

KeywordMonitor *KeywordMonitor::self = nullptr;

KeywordMonitor::KeywordMonitor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KeywordMonitor)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Tool);
}

void KeywordMonitor::initialize(QWidget *parent)
{
    if (!self) {
        self = new KeywordMonitor(parent);
    }
}

KeywordMonitor *KeywordMonitor::instance()
{
    return self;
}

void KeywordMonitor::on_toolButtonAdd_clicked()
{
    ui->tableView->appendRow();
}

void KeywordMonitor::on_toolButtonDelete_clicked()
{

}
