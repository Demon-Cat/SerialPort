#include "MonitorTableView.h"
#include <QHeaderView>

MonitorTableView::MonitorTableView(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setHighlightSections(false);
    verticalHeader()->setVisible(false);

    setSelectionMode(SingleSelection);
    setSelectionBehavior(SelectRows);

    m_model = new MonitorItemModel(this);
    setModel(m_model);

    m_delegate = new MonitorItemDelegate(this);
    setItemDelegate(m_delegate);

    setMouseTracking(true);
}

void MonitorTableView::appendRow()
{
    m_model->insertRow(m_model->rowCount());
}
