#ifndef SELECTCOLOR_H
#define SELECTCOLOR_H

#include <QToolButton>

class SelectColor : public QToolButton
{
    Q_OBJECT
public:
    explicit SelectColor(QWidget *parent = nullptr);

    const QColor &color() const;
    void setColor(const QColor &newColor);

protected:
    void paintEvent(QPaintEvent *) override;

signals:

private:
    QColor m_color = Qt::red;
};

#endif // SELECTCOLOR_H
