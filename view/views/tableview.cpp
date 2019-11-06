#include "tableview.h"

#include <QMouseEvent>
#include <QLoggingCategory>

TableView::TableView(QWidget *parent)
    : QTableView (parent)
{
    setMouseTracking(true);
}

void TableView::mouseMoveEvent(QMouseEvent *event)
{
    int currentRow = indexAt(event->pos()).row();
    if (currentRow != prevHighlightedRow)
    {
        prevHighlightedRow = currentRow;
        emit highlightedRowChange(currentRow);
        viewport()->repaint();
    }
}
