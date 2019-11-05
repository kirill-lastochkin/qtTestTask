#include "customersdelegate.h"
#include "databasemaintainer.h"
#include "validationtools.h"

#include <QLoggingCategory>

bool CustomersDelegate::validateValue(const QSqlTableModel *sourceModel, const QSqlRecord&, int column, const QVariant &newValue) const
{
    switch (column)
    {
    case CustomersColumns::company:
        return validateCompany(newValue.toString(), sourceModel);

    case CustomersColumns::country:
        return validateCountry(newValue.toString());

    case CustomersColumns::address:
        return validateAddress(newValue.toString());

    default:
        qWarning() << "Unexpected column: " << column;
        break;
    }

    return false;
}

bool CustomersDelegate::validateCompany(const QString &newValue, const QSqlTableModel* model) const
{
    qDebug() << "Validating company name" << newValue;

    if (checkEmptyValue(newValue))
        return false;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::companyNameMaxLen))
        return false;

    if (checkKeyDuplication(model, newValue, DatabaseMaintainer::customersKeyName))
        return false;

    return true;
}

bool CustomersDelegate::validateCountry(const QString &newValue) const
{
    qDebug() << "Validating country name" << newValue;

    if (checkEmptyValue(newValue))
        return false;

    if (!isLiteral(newValue))
    {
        qDebug() << "Inserted value" << newValue << "has non-literal symbols";
        return false;
    }

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::countryNameMaxLen))
        return false;

    return true;
}

bool CustomersDelegate::validateAddress(const QString &newValue) const
{
    qDebug() << "Validating address" << newValue;

    if (checkEmptyValue(newValue))
        return false;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::addressMaxLen))
        return false;

    return true;
}
