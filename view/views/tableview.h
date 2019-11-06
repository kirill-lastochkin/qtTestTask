#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>

#include "fetchmodelinfo.h"

class TableView : public QTableView
{
    Q_OBJECT

public:
    explicit TableView(QWidget *parent = nullptr);

signals:
    void highlightedRowChange(int newRow);

private:
    int prevHighlightedRow = -1;
    void mouseMoveEvent(QMouseEvent *event) override;

    inline QSortFilterProxyModel* proxyModel(void) { return viewProxyModel(this); }
    inline QSqlTableModel* sourceModel(void) { return viewSourceModel(this); }
};
#endif // CUSTOMTABLEVIEW_H
