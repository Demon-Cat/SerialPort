#include "AddButton.h"
#include "ui_AddButton.h"
#include <QColorDialog>
#include <QMessageBox>

AddButton::AddButton(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddButton)
{
    ui->setupUi(this);
}

AddButton::~AddButton()
{
    delete ui;
}

const ShortcutInfo &AddButton::info() const
{
    return m_info;
}

void AddButton::setInfo(const ShortcutInfo &newInfo)
{
    m_info = newInfo;
}

void AddButton::on_toolButtonColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::red, this, "选择颜色");
    if (color.isValid()) {
        ui->toolButtonColor->setColor(color);
    }
}

void AddButton::on_pushButtonOk_clicked()
{
    m_info.setTitle(ui->lineEditTitle->text());
    m_info.setCommond(ui->lineEditCommond->text());
    m_info.setColor(ui->toolButtonColor->color());

    if (m_info.title().isEmpty()) {
        QMessageBox::information(this, "提示", "标题不能为空！");
        return;
    }
    if (m_info.commond().isEmpty()) {
        QMessageBox::information(this, "提示", "命令不能为空！");
        return;
    }
    if (!m_info.color().isValid()) {
        QMessageBox::information(this, "提示", "颜色值无效！");
        return;
    }

    accept();
}

void AddButton::on_pushButtonCancel_clicked()
{
    reject();
}
