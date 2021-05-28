#include "BaseTableView.h"
#include <QHeaderView>
#include <QtDebug>
#include <QMouseEvent>

BaseTableView::BaseTableView(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setHighlightSections(false);
    verticalHeader()->setVisible(false);

    setSelectionMode(SingleSelection);
    setSelectionBehavior(SelectRows);

    connect(this, &QTableView::clicked, this, &BaseTableView::onClicked);
    connect(this, &QTableView::doubleClicked, this, &BaseTableView::onDoubleClicked);
}

BaseTableView::~BaseTableView()
{
}

int BaseTableView::selectedRow() const
{
    const QModelIndexList &list = selectedIndexes();
    if (list.isEmpty()) {
        return -1;
    }
    return list.first().row();
}

void BaseTableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);

    const QModelIndex &index = indexAt(event->pos());
    if (!index.isValid()) {
        clearSelection();
    }
}

void BaseTableView::onClicked(const QModelIndex &index)
{
    emit clicked(index.row(), index.column());
}

void BaseTableView::onDoubleClicked(const QModelIndex &index)
{
    emit doubleClicked(index.row(), index.column());
}
