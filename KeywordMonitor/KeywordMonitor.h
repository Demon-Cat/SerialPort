#ifndef KEYWORDMONITOR_H
#define KEYWORDMONITOR_H

#include <QDialog>

#define gKeyworkMonitor KeywordMonitor::instance()

namespace Ui {
class KeywordMonitor;
}

class KeywordMonitor : public QDialog
{
    Q_OBJECT
public:
    explicit KeywordMonitor(QWidget *parent = nullptr);

    static void initialize(QWidget *parent);
    static KeywordMonitor *instance();

signals:

private slots:
    void on_toolButtonAdd_clicked();

    void on_toolButtonDelete_clicked();

private:
    static KeywordMonitor *self;

    Ui::KeywordMonitor *ui;
};

#endif // KEYWORDMONITOR_H
