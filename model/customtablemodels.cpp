#include "customtablemodels.h"
#include "databasemaintainer.h"

SqlTableModelCustomers::SqlTableModelCustomers(QObject *parent, const QSqlDatabase &db)
    : QSqlTableModel(parent, db)
{
    setTable(DatabaseMaintainer::customersTableName);
    setHeaderData(0, Qt::Horizontal, QObject::tr("Company"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("Country"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("Address"));
    setEditStrategy(QSqlTableModel::OnFieldChange);
    select();
}

SqlTableModelProjects::SqlTableModelProjects(QObject *parent, const QSqlDatabase &db)
    : QSqlTableModel(parent, db)
{
    setTable(DatabaseMaintainer::projectsTableName);
    setHeaderData(0, Qt::Horizontal, QObject::tr("Project"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("Customer"));
    setHeaderData(3, Qt::Horizontal, QObject::tr("Start date"));
    setHeaderData(4, Qt::Horizontal, QObject::tr("End date"));
    setEditStrategy(QSqlTableModel::OnFieldChange);
    select();
}
