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
    friend class ProjectInfoWindow;
    explicit CommonTab(QWidget *parent = nullptr);

    void setTableModel(QSqlTableModel *model);

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


class CustomersTab : public CommonTab
{
    Q_OBJECT

public:
    explicit CustomersTab(QWidget *parent = nullptr);
};


class ProjectsTab : public CommonTab
{
    Q_OBJECT

public:
    explicit ProjectsTab(QWidget *parent = nullptr);

    void setTableModel(QSqlTableModel *model);

public slots:
    void showProjectInfo(void);
    void hideProjectInfo(ProjectInfoWindow *obj);

    void setProjectFilterByCustomer(const QString &newText);

private:
    QLineEdit *editCustomerFilter;
    const int maxProjectInfoWinNumber = 4;
    static int openedWinCount;
};

#endif // TAB_H