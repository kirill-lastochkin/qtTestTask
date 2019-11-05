#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModelCustomers : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SqlTableModelCustomers(QObject *parent, const QSqlDatabase &db);
};

class SqlTableModelProjects : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SqlTableModelProjects(QObject *parent, const QSqlDatabase &db);
};

#endif // CUSTOMTABLEMODEL_H
