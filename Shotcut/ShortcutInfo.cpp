#include "ShortcutInfo.h"

ShortcutInfo::ShortcutInfo()
{

}

const QString &ShortcutInfo::title() const
{
    return m_title;
}

void ShortcutInfo::setTitle(const QString &newTitle)
{
    m_title = newTitle;
}

const QString &ShortcutInfo::commond() const
{
    return m_commond;
}

void ShortcutInfo::setCommond(const QString &newCommond)
{
    m_commond = newCommond;
}

const QColor &ShortcutInfo::color() const
{
    return m_color;
}

void ShortcutInfo::setColor(const QColor &newColor)
{
    m_color = newColor;
}

const QString ShortcutInfo::colorString() const
{
    return m_color.name();
}

void ShortcutInfo::setColorString(const QString &newColor)
{
    m_color.setNamedColor(newColor);
}

int ShortcutInfo::id() const
{
    return m_id;
}

void ShortcutInfo::setId(int newIndex)
{
    m_id = newIndex;
}
