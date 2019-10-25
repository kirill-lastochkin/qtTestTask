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

class CommonTab : public QWidget
{
    Q_OBJECT

    friend class RowSelection;

public:
    explicit CommonTab(QWidget *parent = nullptr);

    virtual void setTableModel(QSqlTableModel *model);

public slots:
    void addRow(void);
    void delRow(void);
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
    virtual void selectRowByKey(const QString &key) = 0;
    virtual void deselectRow(void) = 0;

    bool confirmDeletion(void);
    inline void checkEmptyModel(const QSqlTableModel *model);

    inline QSortFilterProxyModel* proxyModel(void)
    {
        return reinterpret_cast<QSortFilterProxyModel*>(tableView->model());
    }

    inline QSqlTableModel* sourceModel(void)
    {
        return reinterpret_cast<QSqlTableModel*>(proxyModel()->sourceModel());
    }

    inline QModelIndex getNextIndex(const QModelIndex &index);
};

class RowSelection
{
public:
    RowSelection() = delete;
    explicit RowSelection(CommonTab *tab, const QString &key)
        : owner(tab)
    {
        tab->selectRowByKey(key);
    }
    ~RowSelection()
    {
        owner->deselectRow();
    }

private:
    CommonTab *owner;
};

#endif // TAB_H
