#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableView>

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
};
#endif // CUSTOMTABLEVIEW_H
