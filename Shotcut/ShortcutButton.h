#ifndef SHORTCUTBUTTON_H
#define SHORTCUTBUTTON_H

#include <QPushButton>
#include "ShortcutInfo.h"

class ShortcutButton : public QPushButton
{
    Q_OBJECT

    enum State {
        StateNone,
        StateHover,
        StatePressed
    };

public:
    explicit ShortcutButton(const ShortcutInfo &info, QWidget *parent = nullptr);

    void setInfo(const ShortcutInfo &info);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *) override;

    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

signals:
    void input(const QByteArray &data);

private slots:
    void onClicked();

private:
    ShortcutInfo m_info;
    State m_state = StateNone;
};

#endif // SHORTCUTBUTTON_H
