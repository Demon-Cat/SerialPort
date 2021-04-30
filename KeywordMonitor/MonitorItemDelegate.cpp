#include "MonitorItemDelegate.h"
#include "MonitorItemModel.h"
#include <QApplication>
#include <QComboBox>
#include <QMouseEvent>
#include <QtDebug>

MonitorItemDelegate::MonitorItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *MonitorItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    switch (index.column()) {
    case MonitorItemModel::ColumnEnable:
        return nullptr;
    case MonitorItemModel::ColumnType: {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItem("字符串匹配", MonitorInfo::MatchString);
        comboBox->addItem("正则匹配", MonitorInfo::MatchRegular);
        return comboBox;
    }
    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void MonitorItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column()) {
    case MonitorItemModel::ColumnType: {
        QComboBox *comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(comboBox->findData(index.data(Qt::UserRole)));
        break;;
    }
    default:
        return QStyledItemDelegate::setEditorData(editor, index);
    }
}

void MonitorItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column()) {
    case MonitorItemModel::ColumnType: {
        QComboBox *comboBox = static_cast<QComboBox *>(editor);
        model->setData(index, comboBox->currentData(), Qt::EditRole);
        break;;
    }
    default:
        return QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void MonitorItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    if (option.state.testFlag(QStyle::State_HasFocus)) {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);

    switch (index.column()) {
    case MonitorItemModel::ColumnEnable: {
        QStyleOptionButton optionCheckBox;
        optionCheckBox.state |= QStyle::State_Enabled;
        if (index.data(Qt::UserRole).toBool()) {
            optionCheckBox.state |= QStyle::State_On;
        } else {
            optionCheckBox.state |= QStyle::State_Off;
        }
        switch (m_mouseState) {
        case StateHover:
            optionCheckBox.state |= QStyle::State_MouseOver;
            break;
        case StatePressed:
            optionCheckBox.state |= QStyle::State_Sunken;
            break;
        case StateReleased:
            break;
        default:
            break;
        }

        optionCheckBox.rect = checkBoxRect(option);
        qApp->style()->drawControl(QStyle::CE_CheckBox, &optionCheckBox, painter);
        break;
    }
    default:
        break;
    }
}

bool MonitorItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    qDebug() << index;
    switch (index.column()) {
    case MonitorItemModel::ColumnEnable:
        switch (event->type()) {
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (checkBoxRect(option).contains(mouseEvent->pos())) {
                m_mouseState = StateHover;
            } else {
                m_mouseState = StateNone;
            }
            return true;
        }
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (checkBoxRect(option).contains(mouseEvent->pos())) {
                m_mouseState = StatePressed;
                bool checked = index.data(Qt::UserRole).toBool();
                model->setData(index, !checked, Qt::UserRole);
                return true;
            } else {
                m_mouseState = StateNone;
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (checkBoxRect(option).contains(mouseEvent->pos())) {
                m_mouseState = StateHover;
                return true;
            } else {
                m_mouseState = StateNone;
            }
            break;
        }
        default:
            break;
        }
        break;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QRect MonitorItemDelegate::checkBoxRect(const QStyleOptionViewItem &option) const
{
    QRect rc = qApp->style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option);
    rc.moveCenter(option.rect.center());
    return rc;
}
