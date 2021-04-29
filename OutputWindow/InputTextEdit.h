#ifndef INPUTTEXTEDIT_H
#define INPUTTEXTEDIT_H

#include <QPlainTextEdit>

class InputTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit InputTextEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *e) override;

signals:
    void enterPressed();
};

#endif // INPUTTEXTEDIT_H
