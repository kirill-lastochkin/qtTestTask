#ifndef TAB_H
#define TAB_H

#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "databasemaintainer.h"
#include "projectinfowindow.h"
#include "tableeditdelegate.h"
#include "customtableview.h"

class CommonTab : public QWidget
{
    Q_OBJECT

public:
    explicit CommonTab(QWidget *parent = nullptr);

    virtual void setTableModel(QSqlTableModel *model);

public slots:
    void addRow(void);
    void delRow(void);
    void askDelRow(void);
    void setEmptyDbImage(void);

private slots:
    void editTableItemFailed(const QModelIndex &index);
    void editTableItemSucceeded(const QModelIndex &index);

signals:
    void setActive(bool);
    void setActiveTableWidget(QWidget*);

protected:
    TableView *tableView;
    QLabel *emptyDbLabel;

    QVBoxLayout *tabLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *spaceLayout;

    TableEditDelegate *editDelegate;

    static const int buttonWidthMax;
    static const char * const emptyDbImagePath;

protected:
    inline QSortFilterProxyModel* getProxyModel(void)
    {
        return reinterpret_cast<QSortFilterProxyModel*>(tableView->model());
    }

    QSqlTableModel* getSourceModel(void)
    {
        return reinterpret_cast<QSqlTableModel*>(getProxyModel()->sourceModel());
    }
};

#endif // TAB_H
