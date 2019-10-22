#include "projectsdelegate.h"
#include "databasemaintainer.h"
#include "validationtools.h"

#include <QLoggingCategory>

bool ProjectsDelegate::validateValue(const QSqlTableModel *sourceModel, const QSqlRecord &oldRecord, int column, const QVariant &newValue) const
{
    using TableColumn = DatabaseMaintainer::ProjectsTableColumn;

    switch (column)
    {
    case TableColumn::project:
        return validateProject(newValue.toString(), sourceModel);

    case TableColumn::customer:
        return validateCustomer(newValue.toString());

    case TableColumn::description:
        return validateDescription(newValue.toString());

    case TableColumn::startDate:
        return validateStartDate(newValue.toDate(), oldRecord);

    case TableColumn::endDate:
        return validateEndDate(newValue.toDate(), oldRecord);

    default:
        break;
    }

    return false;
}

bool ProjectsDelegate::validateProject(const QString &newValue, const QSqlTableModel* model) const
{
    qDebug() << "Validating project name" << newValue;

    if (checkEmptyValue(newValue))
        return false;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::projectNameMaxLen))
        return false;

    if (checkKeyDuplication(model, newValue, DatabaseMaintainer::projectsKeyName))
        return false;

    return true;
}

bool ProjectsDelegate::validateCustomer(const QString &newValue) const
{
    qDebug() << "Validating customer name" << newValue;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::companyNameMaxLen))
        return false;

    return true;
}

bool ProjectsDelegate::validateDescription(const QString &newValue) const
{
    qDebug() << "Validating description" << newValue;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::descriptionMaxLen))
        return false;

    return true;
}

bool ProjectsDelegate::validateStartDate(const QDate &newValue, const QSqlRecord &record) const
{
    qDebug() << "Validating start date" << newValue.toString(DatabaseMaintainer::dateFormat);

    auto currentEndDate = record.value(DatabaseMaintainer::projectsEndDateName).toDate();

    if (newValue.isNull())
    {
        qDebug() << "Wrong date format, must be" << DatabaseMaintainer::dateFormat;
        return false;
    }

    if (!currentEndDate.isNull() && newValue > currentEndDate)
    {
        qDebug() << "Start date" << newValue.toString(DatabaseMaintainer::dateFormat) << "is greater, than end date" << currentEndDate.toString(DatabaseMaintainer::dateFormat);
        return false;
    }

    return true;
}

bool ProjectsDelegate::validateEndDate(const QDate &newValue, const QSqlRecord &record) const
{
    qDebug() << "Validating end date" << newValue.toString(DatabaseMaintainer::dateFormat);

    auto currentStartDate = record.value(DatabaseMaintainer::projectsStartDateName).toDate();

    if (newValue.isNull())
    {
        qDebug() << "Wrong date format, must be" << DatabaseMaintainer::dateFormat;
        return false;
    }

    if (newValue < currentStartDate)
    {
        qDebug() << "End date" << newValue.toString(DatabaseMaintainer::dateFormat) << "is less, than start date" << currentStartDate.toString(DatabaseMaintainer::dateFormat);
        return false;
    }

    return true;
}
