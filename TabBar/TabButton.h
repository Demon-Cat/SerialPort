#ifndef TABBUTTON_H
#define TABBUTTON_H

#include <QToolButton>

class TabButton : public QToolButton
{
    Q_OBJECT
public:
    enum State {
        StateNone,
        StateConnected,
        StateDisconnected
    };

    explicit TabButton(QWidget *parent = nullptr);

    State state() const;
    void setState(State newState);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *) override;

signals:
    void tabClicked(const QString &text);
    void tabCloseRequested(const QString &text);

private slots:
    void onClicked();
    void onCloseClicked();

private:
    QToolButton *m_closeButton = nullptr;

    State m_state = StateNone;
};

#endif // TABBUTTON_H
