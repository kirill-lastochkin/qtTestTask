#include <QSqlError>
#include <QSqlQuery>
#include <QLoggingCategory>
#include <QSqlTableModel>

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

bool DatabaseMaintainer::updateProject(const QSqlTableModel *db, const ProjectInfo &info)
{
    QSqlQuery query(db->database());

    auto str = QString("update %1 set %2 = '%3', %4 = '%5', %6 = '%7', %8 = '%9' where %10 = '%11'")\
            .arg(projectsTableName)\
            .arg(projectsCustomerName).arg(info.customer)\
            .arg(projectsDescName).arg(info.description)\
            .arg(projectsStartDateName).arg(info.start.toString(DatabaseMaintainer::dateFormat))\
            .arg(projectsEndDateName).arg(info.end.toString(DatabaseMaintainer::dateFormat))\
            .arg(projectsKeyName).arg(info.project);

    bool r = query.exec(str);
    if (!r)
    {
        qDebug() << query.lastError();
    }
    return r;
}

const QSqlRecord DatabaseMaintainer::getProject(const QSqlTableModel *db, const QString &project)
{
    QSqlQuery query(db->database());
    bool isOk = query.exec(QString("select * from %1 where %2 = '%3'").arg(projectsTableName).arg(projectsKeyName).arg(project));
    if (!isOk)
    {
        qWarning() << query.lastError();
        return QSqlRecord();
    }

    int resultCnt = 0;
    while (query.next())
        resultCnt++;

    if (resultCnt != 1)
    {
        qDebug() << resultCnt << "records were found by key:" << project;
        return QSqlRecord();
    }

    query.first();
    return query.record();
}


