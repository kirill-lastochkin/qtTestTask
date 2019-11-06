#include "projectinfo.h"
#include "databasemaintainer.h"
#include <QVariant>
#include <QLoggingCategory>

bool checkSqlRecordValid(const QSqlRecord &record);

ProjectInfo::ProjectInfo(const QSqlRecord &record)
{
    if (checkSqlRecordValid(record))
    {
        project = record.value(ProjectsColumns::project).toString();
        customer = record.value(ProjectsColumns::customer).toString();
        description = record.value(ProjectsColumns::description).toString();
        start = record.value(ProjectsColumns::startDate).toDate();
        end = record.value(ProjectsColumns::endDate).toDate();
    }
}

ProjectInfo::ProjectInfo(const ProjectInfo &other)
    : ProjectInfo(other.project, other.customer, other.description, other.start, other.end)
{}

ProjectInfo::ProjectInfo(const QString &argProject, const QString &argCustomer, const QString &argDesc,
                         const QDate &argStart, const QDate &argEnd)
    : project(argProject), customer(argCustomer), description(argDesc), start(argStart), end(argEnd)
{}

QVariant ProjectInfo::operator[](int index) const
{
    switch (index)
    {
    case ProjectsColumns::project:
        return QVariant(project);

    case ProjectsColumns::customer:
        return QVariant(customer);

    case ProjectsColumns::description:
        return QVariant(description);

    case ProjectsColumns::startDate:
        return QVariant(start);

    case ProjectsColumns::endDate:
        return QVariant(end);

    default:
        qWarning() << "Index out of borders" << index;
        return QVariant();
    }
}

bool checkSqlRecordValid(const QSqlRecord &record)
{
    if (record.count() != ProjectsColumns::projectsTableColumnCount)
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
