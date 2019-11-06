#include <QSqlError>
#include <QSqlQuery>
#include <QLoggingCategory>
#include <QSqlTableModel>

#include "databasemaintainer.h"
#include "postmessage.h"

DatabaseMaintainer::DatabaseMaintainer()
    : db(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connectionName)))
{}

DatabaseMaintainer::DatabaseMaintainer(const QString &dbName)
    : DatabaseMaintainer()
{
    connectDatabase(dbName);
}

DatabaseMaintainer::~DatabaseMaintainer()
{
    if (db->isOpen())
        db->close();

    delete db;

    QSqlDatabase::removeDatabase(connectionName);
}

bool DatabaseMaintainer::connectDatabase(const QString &dbName)
{
    db->setDatabaseName(dbName);
    if (!db->open())
    {
        showMessage(QString("Unaible to open DB %1").arg(dbName));
        state = DatabaseState::ConnectionFailed;
        return false;
    }

    state = DatabaseState::Connected;
    return true;
}

bool DatabaseMaintainer::makeQuery(const QString &queryString)
{
    QSqlQuery query(*db);
    bool isOk = query.exec(queryString);
    if (!isOk)
        qWarning() << query.lastError();

    return  isOk;
}

bool DatabaseMaintainer::createCustomersTable(void)
{
    bool isOk = makeQuery(QString("drop table if exists %1").arg(customersTableName));
    isOk = makeQuery(QString("create table %1 (%2 varchar(%3) primary key, %4 varchar(%5), %6 varchar(%7))")\
                     .arg(customersTableName)\
                     .arg(customersKeyName).arg(companyNameMaxLen)\
                     .arg(customersCountryName).arg(countryNameMaxLen)\
                     .arg(customersAddressName).arg(addressMaxLen));

    state = isOk ? DatabaseState::Ready : DatabaseState::Failed;
    return  isOk;
}

bool DatabaseMaintainer::insertCustomer(const CustomerInfo &info)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("insert into %1 values('%2', '%3', '%4')").arg(customersTableName)\
                             .arg(info.company).arg(info.country).arg(info.address));
}

bool DatabaseMaintainer::removeCustomer(const QString &company)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("delete from %1 where company = '%2'").arg(customersTableName).arg(company));
}


bool DatabaseMaintainer::createProjectsTable(void)
{
    bool isOk = makeQuery(QString("drop table if exists %1").arg(projectsTableName));
    isOk = makeQuery(QString("create table %1 (%2 varchar(%3) primary key, %4 varchar(%5), %6 "
                              "varchar(%7), %8 date, %9 date)").arg(projectsTableName)\
                              .arg(projectsKeyName).arg(projectNameMaxLen)\
                              .arg(projectsCustomerName).arg(companyNameMaxLen)\
                              .arg(projectsDescName).arg(descriptionMaxLen)\
                              .arg(projectsStartDateName).arg(projectsEndDateName));
    state = isOk ? DatabaseState::Ready : DatabaseState::Failed;
    return isOk;
}

bool DatabaseMaintainer::insertProject(const ProjectInfo &info)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("insert into %1 values('%2', '%3', '%4', '%5', '%6')").arg(projectsTableName)\
                     .arg(info.project).arg(info.customer).arg(info.description)\
                     .arg(info.start.toString(dateFormat)).arg(info.end.toString(dateFormat)));
}

bool DatabaseMaintainer::removeProject(const QString &project)
{
    if (state != DatabaseState::Ready)
        return false;

    return makeQuery(QString("delete from %1 where %2 = '%3'").arg(projectsTableName).arg(projectsKeyName).arg(project));
}
