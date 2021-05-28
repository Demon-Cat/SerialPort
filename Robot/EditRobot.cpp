#include "EditRobot.h"
#include "ui_EditRobot.h"
#include "Database.h"
#include <QMessageBox>

EditRobot::EditRobot(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditRobot)
{
    ui->setupUi(this);
}

EditRobot::~EditRobot()
{
    delete ui;
}

int EditRobot::execEdit(RobotInfo *info)
{
    m_info = info;

    ui->lineEditId->setText(QString("%1").arg(m_info->id()));
    ui->lineEditName->setText(m_info->name());
    ui->plainTextEditWebhook->setPlainText(m_info->webhook());
    ui->plainTextEditNote->setPlainText(m_info->note());

    return exec();
}

void EditRobot::on_pushButtonOk_clicked()
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
    info.setId(m_info->id());
    info.setName(name);
    info.setWebhook(webhook);
    info.setNote(note);

    int result = gDatabase.updateRobot(info);
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

void EditRobot::on_pushButtonCancel_clicked()
{
    reject();
}
