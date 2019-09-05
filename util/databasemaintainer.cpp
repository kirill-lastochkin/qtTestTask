#include <QSqlError>
#include <QSqlQuery>
#include <QLoggingCategory>

#include "databasemaintainer.h"

DatabaseMaintainer::DatabaseMaintainer()
    : db(QSqlDatabase::addDatabase("QSQLITE", connectionName))
{}

DatabaseMaintainer::DatabaseMaintainer(const QString &dbName)
    : DatabaseMaintainer()
{
    connectDatabase(dbName);
}

DatabaseMaintainer::~DatabaseMaintainer()
{
    if (db.isOpen())
        db.close();

    QSqlDatabase::removeDatabase(connectionName);
}

bool DatabaseMaintainer::connectDatabase(const QString &dbName)
{
    db.setDatabaseName(dbName);
    if (!db.open()) {
        qCritical() << "Unaible to open DB " << dbName << "\n";
        state = DatabaseState::ConnectionFailed;
        return false;
    }

    state = DatabaseState::Connected;
    return true;
}

bool DatabaseMaintainer::makeQuery(const QString &queryString)
{
    QSqlQuery query(db);
    bool isOk = query.exec(queryString);
    if (!isOk)
        qWarning() << query.lastError();

    return  isOk;
}

bool DatabaseMaintainer::createCustomersTable(void)
{
    bool isOk = makeQuery(QString("drop table if exists %1").arg(DatabaseMaintainer::customersTableName));
    isOk = makeQuery(QString("create table %1 (%5 varchar(%2) primary key, country varchar(%3), "
                     "address varchar(%4))").arg(DatabaseMaintainer::customersTableName)\
                     .arg(companyNameMaxLen).arg(countryNameMaxLen).arg(addressMaxLen)\
                     .arg(DatabaseMaintainer::customersKeyName));
    state = isOk ? DatabaseState::Ready : DatabaseState::Failed;
    return  isOk;
}

bool DatabaseMaintainer::insertCustomer(const CustomerInfo &info)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("insert into %1 values('%2', '%3', '%4')").arg(DatabaseMaintainer::customersTableName)\
                             .arg(info.company).arg(info.country).arg(info.address));
}

bool DatabaseMaintainer::removeCustomer(const QString &company)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("delete from %1 where company = '%2'").arg(DatabaseMaintainer::customersTableName).arg(company));
}


bool DatabaseMaintainer::createProjectsTable(void)
{
    bool isOk = makeQuery(QString("drop table if exists %1").arg(DatabaseMaintainer::projectsTableName));
    isOk = makeQuery(QString("create table %1 (%2 varchar(%3) primary key, customer varchar(%4), description "
                              "varchar(%5), %6 date, %7 date)").arg(DatabaseMaintainer::projectsTableName)\
                              .arg(DatabaseMaintainer::projectsKeyName).arg(projectNameMaxLen).arg(companyNameMaxLen)\
                              .arg(descriptionMaxLen).arg(DatabaseMaintainer::projectsStartDateName)\
                              .arg(DatabaseMaintainer::projectsEndDateName));
    state = isOk ? DatabaseState::Ready : DatabaseState::Failed;
    return isOk;
}

bool DatabaseMaintainer::insertProject(const ProjectInfo &info)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("insert into %1 values('%2', '%3', '%4', '%5', '%6')").arg(DatabaseMaintainer::projectsTableName)\
                     .arg(info.project).arg(info.customer).arg(info.description)\
                     .arg(info.start.toString(Qt::ISODate)).arg(info.end.toString(Qt::ISODate)));
}

bool DatabaseMaintainer::removeProject(const QString &project)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("delete from %1 where project = '%2'").arg(DatabaseMaintainer::projectsTableName).arg(project));
}
