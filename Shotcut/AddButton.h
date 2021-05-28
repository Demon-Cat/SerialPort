#ifndef ADDBUTTON_H
#define ADDBUTTON_H

#include <QDialog>
#include "ShortcutInfo.h"

namespace Ui {
class AddButton;
}

class AddButton : public QDialog
{
    Q_OBJECT

public:
    explicit AddButton(QWidget *parent = nullptr);
    ~AddButton();

    const ShortcutInfo &info() const;
    void setInfo(const ShortcutInfo &newInfo);

private slots:
    void on_toolButtonColor_clicked();

    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::AddButton *ui;

    ShortcutInfo m_info;
};

#endif // ADDBUTTON_H
