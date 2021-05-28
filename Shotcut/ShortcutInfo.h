#ifndef SHORTCUTINFO_H
#define SHORTCUTINFO_H

#include <QString>
#include <QColor>

class ShortcutInfo
{
public:
    ShortcutInfo();

    const QString &title() const;
    void setTitle(const QString &newTitle);

    const QString &commond() const;
    void setCommond(const QString &newCommond);

    const QColor &color() const;
    void setColor(const QColor &newColor);

    const QString colorString() const;
    void setColorString(const QString &newColor);

    int id() const;
    void setId(int newId);

private:
    QString m_title;
    QString m_commond;
    QColor m_color;
    int m_id = -1;
};

#endif // SHORTCUTINFO_H
