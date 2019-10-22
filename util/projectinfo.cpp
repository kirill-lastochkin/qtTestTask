#include "projectinfo.h"
#include "databasemaintainer.h"
#include <QVariant>
#include <QLoggingCategory>

bool checkSqlRecordValid(const QSqlRecord &record);

using TableColumn = DatabaseMaintainer::ProjectsTableColumn;

ProjectInfo::ProjectInfo(const QSqlRecord &record)
{
    if (checkSqlRecordValid(record))
    {
        project = record.value(TableColumn::project).toString();
        customer = record.value(TableColumn::customer).toString();
        description = record.value(TableColumn::description).toString();
        start = record.value(TableColumn::startDate).toDate();
        end = record.value(TableColumn::endDate).toDate();
    }
}

ProjectInfo::ProjectInfo(const QString &argProject, const QString &argCustomer, const QString &argDesc,
                         const QDate &argStart, const QDate &argEnd)
    : project(argProject), customer(argCustomer), description(argDesc), start(argStart), end(argEnd)
{}

QVariant ProjectInfo::operator[](int index)
{
    switch (index)
    {
    case TableColumn::project:
        return QVariant(project);

    case TableColumn::customer:
        return QVariant(customer);

    case TableColumn::description:
        return QVariant(description);

    case TableColumn::startDate:
        return QVariant(start);

    case TableColumn::endDate:
        return QVariant(end);

    default:
        qWarning() << "Index out of borders" << index;
        return QVariant();
    }
}

bool checkSqlRecordValid(const QSqlRecord &record)
{
    if (record.count() != TableColumn::projectsTableColumnCount)
    {
        qWarning() << "Invalid sql record with" << record.count() << "fields";
        return false;
    }

    if (record.indexOf(DatabaseMaintainer::projectsKeyName) == -1 ||
        record.indexOf(DatabaseMaintainer::projectsCustomerName) == -1 ||
        record.indexOf(DatabaseMaintainer::projectsDescName) == -1 ||
        record.indexOf(DatabaseMaintainer::projectsStartDateName) == -1 ||
        record.indexOf(DatabaseMaintainer::projectsEndDateName) == -1)
    {
        qWarning() << "Invalid sql record with wrong fields names" << record;
        return false;
    }

    return true;
}

#if 0
bool ProjectInfo::operator==(const ProjectInfo &other) const
{
    if (project != other.project || customer != other.customer || description != other.description ||
        start != other.start || end != other.end)
    {
        return false;
    }

    return true;
}

bool ProjectInfo::fillSqlRecord(QSqlRecord &record)
{
    if (!checkSqlRecordValid(record))
        return false;

    record.setValue(TableColumn::project, project);
    record.setValue(TableColumn::customer, customer);
    record.setValue(TableColumn::description, description);
    record.setValue(TableColumn::startDate, start);
    record.setValue(TableColumn::endDate, end);

    return true;
}
#endif
