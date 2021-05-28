#include "AddRobot.h"
#include "ui_AddRobot.h"
#include "Database.h"
#include <QMessageBox>

AddRobot::AddRobot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRobot)
{
    ui->setupUi(this);
}

AddRobot::~AddRobot()
{
    delete ui;
}

void AddRobot::on_pushButtonOk_clicked()
{
    QString name = ui->lineEditName->text();
    QString webhook = ui->plainTextEditWebhook->toPlainText();
    QString note = ui->plainTextEditNote->toPlainText();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "提示", "机器人名字不能为空！");
        return;
    }
    if (webhook.isEmpty()) {
        QMessageBox::warning(this, "提示", "Webhook地址不能为空！");
        return;
    }

    RobotInfo info;
    info.setName(name);
    info.setWebhook(webhook);
    info.setNote(note);

    int result = gDatabase.insertRobot(info);
    switch (result) {
    case Database::NoError:
        break;
    case Database::QueryError:
        QMessageBox::warning(this, "提示", "写数据库错误！");
        return;
    case Database::RobotNameExist:
        QMessageBox::warning(this, "提示", "机器人名字已存在！");
        return;
    case Database::RobotWebhookExist:
        QMessageBox::warning(this, "提示", "Webhook地址已存在！");
        return;
    default:
        break;
    }

    accept();
}

void AddRobot::on_pushButtonCancel_clicked()
{
    reject();
}

