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
#include "tableview.h"
#include "fetchmodelinfo.h"

class CommonTab : public QWidget
{
    Q_OBJECT

public:
    explicit CommonTab(QWidget *parent = nullptr);

    virtual void setTableModel(QSqlTableModel *model);

public slots:
    void addRow(int newRow);
    void delRows(void);
    void setEmptyDbImage(void);

private slots:
    void editTableItemFailed(const QModelIndex &index);
    void editTableItemSucceeded(const QModelIndex &index);

signals:
    void setActive(bool);
    void setActiveTableWidget(QWidget*);
    void entriesRemoved(const QString &table, QVector<int> &rows);
    void addButtonPressed(const QString &table);
    void addingAllowed(bool);

protected:
    TableView *tableView;
    QLabel *emptyDbLabel;

    QVBoxLayout *tabLayout;
    QHBoxLayout *buttonLayout;

    TableEditDelegate *editDelegate;

    static const int buttonWidthMax;
    static const char * const emptyDbImagePath;

protected:
    bool confirmDeletion(void);
    inline void checkEmptyModel(const QSqlTableModel *model);

    inline QSortFilterProxyModel* proxyModel(void) { return viewProxyModel(tableView); }
    inline QSqlTableModel* sourceModel(void) { return viewSourceModel(tableView); }

    inline QModelIndex getNextIndex(const QModelIndex &index);
};

#endif // TAB_H
