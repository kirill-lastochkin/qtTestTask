#include "tableeditdelegate.h"
#include "databasemaintainer.h"

#include <QLoggingCategory>
#include <QLineEdit>
#include <QRegExp>
#include <QPainter>

bool isLiteral(const QString &str);
bool checkLengthExceeded(const QString &newValue, int len, int maxLen);

void TableEditDelegate::setModelData(QWidget *editor,
                                     QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    auto lineEditor = reinterpret_cast<QLineEdit*>(editor);
    auto proxyModel = getProxyModel(model);

    qDebug() << "Validating new value in table" << getProxySourceSqlTableModel(proxyModel)->tableName()
             << ", in row with key:" << getKeyName(index);

    if (!validateValue(index, lineEditor->text()))
    {
        qDebug() << "Validation failed";
        emit validationFailed(index);
        return;
    }

    emit validationSucceeded(index);

    QStyledItemDelegate::setModelData(editor, model, index);
}

void TableEditDelegate::highlightedRowChanged(int newRow)
{
    highlightedRow = newRow;
}

void TableEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int row = index.row();
    if (row == highlightedRow)
        painter->fillRect(option.rect, Qt::red);
    else if (row % 2 == 1)
        painter->fillRect(option.rect, Qt::lightGray);
    else
        painter->fillRect(option.rect, Qt::white);

    QStyledItemDelegate::paint(painter, option, index);
}

bool CustomerTableEditDelegate::validateValue(const QModelIndex &index, const QString &newValue) const
{
    if (newValue.isEmpty())
    {
        qDebug() << "Trying to set empty value";
        return false;
    }

    auto model = getProxySourceSqlTableModel(getProxyModel(index.model()));

    switch (index.column())
    {
    case DatabaseMaintainer::CustomersTableColumn::company:
        return validateCompany(newValue, model, index.row());

    case DatabaseMaintainer::CustomersTableColumn::country:
        return validateCountry(newValue);

    case DatabaseMaintainer::CustomersTableColumn::address:
        return validateAddress(newValue);

    default:
        qWarning() << "Unexpected column: " << index.column();
        break;
    }

    return false;
}

const QString CustomerTableEditDelegate::getKeyName(const QModelIndex &index) const
{
    auto model = getProxySourceSqlTableModel(getProxyModel(index.model()));
    return model->record(index.row()).value(DatabaseMaintainer::customersKeyName).toString();
}

bool checkKeyDuplication(const QSqlTableModel* model, const QString &key, const QString &keyName, int keyRow)
{
    for (int row = 0; row < model->rowCount(); row++)
    {
        if (row == keyRow)
            continue;

        auto record = model->record(row);
        if (record.value(keyName) == key)
        {
            qDebug() << "Name duplication found '" << key << "' in rows " << row << " and " << keyRow;
            return true;
        }
    }

    return false;
}

bool CustomerTableEditDelegate::validateCompany(const QString &newValue, const QSqlTableModel* model, int currentRow) const
{
    qDebug() << "Validating company name" << newValue;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::companyNameMaxLen))
        return false;

    if (checkKeyDuplication(model, newValue, DatabaseMaintainer::customersKeyName, currentRow))
        return false;

    return true;
}

bool CustomerTableEditDelegate::validateCountry(const QString &newValue) const
{
    qDebug() << "Validating country name" << newValue;

    if (!isLiteral(newValue))
    {
        qDebug() << "Inserted value" << newValue << "has non-literal symbols";
        return false;
    }

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::countryNameMaxLen))
        return false;

    return true;
}

bool CustomerTableEditDelegate::validateAddress(const QString &newValue) const
{
    qDebug() << "Validating address" << newValue;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::addressMaxLen))
        return false;

    return true;
}

bool ProjectTableEditDelegate::validateValue(const QModelIndex &index, const QString &newValue) const
{
    auto model = getProxySourceSqlTableModel(getProxyModel(index.model()));

    if (newValue.isEmpty())
    {
        qDebug() << "Trying to set empty value";
        return false;
    }

    switch (index.column())
    {
    case DatabaseMaintainer::ProjectsTableColumn::project:
    {
        return validateProject(newValue, model, index.row());
    }

    case DatabaseMaintainer::ProjectsTableColumn::customer:
    {
        return validateCustomer(newValue);
    }

    case DatabaseMaintainer::ProjectsTableColumn::description:
    {
        return validateDescription(newValue);
    }

    case DatabaseMaintainer::ProjectsTableColumn::startDate:
    {
        auto newStartDate = QDate::fromString(newValue, DatabaseMaintainer::dateFormat);
        return validateStartDate(newStartDate, model->record(index.row()));
    }

    case DatabaseMaintainer::ProjectsTableColumn::endDate:
    {
        auto newEndDate = QDate::fromString(newValue, DatabaseMaintainer::dateFormat);
        return validateEndDate(newEndDate, model->record(index.row()));
    }

    default:
    {
        break;
    }
    }

    return false;
}

const QString ProjectTableEditDelegate::getKeyName(const QModelIndex &index) const
{
    auto model = getProxySourceSqlTableModel(getProxyModel(index.model()));
    return model->record(index.row()).value(DatabaseMaintainer::projectsKeyName).toString();
}

bool ProjectTableEditDelegate::validateProject(const QString &newValue, const QSqlTableModel* model, int currentRow) const
{
    qDebug() << "Validating project name" << newValue;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::projectNameMaxLen))
        return false;

    if (checkKeyDuplication(model, newValue, DatabaseMaintainer::projectsKeyName, currentRow))
        return false;

    return true;
}

bool ProjectTableEditDelegate::validateCustomer(const QString &newValue) const
{
    qDebug() << "Validating customer name" << newValue;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::companyNameMaxLen))
        return false;

    return true;
}

bool ProjectTableEditDelegate::validateDescription(const QString &newValue) const
{
    qDebug() << "Validating description" << newValue;

    if (checkLengthExceeded(newValue, newValue.length(), DatabaseMaintainer::descriptionMaxLen))
        return false;

    return true;
}

bool ProjectTableEditDelegate::validateStartDate(const QDate &newValue, const QSqlRecord &record) const
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

bool ProjectTableEditDelegate::validateEndDate(const QDate &newValue, const QSqlRecord &record) const
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

bool isLiteral(const QString &str)
{
    QRegExp reg("[^a-z A-Z]");
    return (reg.indexIn(str) == -1);
}

bool checkLengthExceeded(const QString &newValue, int len, int maxLen)
{
    if (len > maxLen)
    {
        qDebug() << "Length limit in string" << newValue << "is broken:" << len << ">" << maxLen;
        return true;
    }

    return false;
}

inline QSortFilterProxyModel* getProxyModel(QAbstractItemModel *model)
{
    return reinterpret_cast<QSortFilterProxyModel*>(model);
}

inline const QSortFilterProxyModel* getProxyModel(const QAbstractItemModel *model)
{
    return reinterpret_cast<const QSortFilterProxyModel*>(model);
}

inline QSqlTableModel* getProxySourceSqlTableModel(QSortFilterProxyModel *proxyModel)
{
    return reinterpret_cast<QSqlTableModel*>(proxyModel->sourceModel());
}

inline const QSqlTableModel* getProxySourceSqlTableModel(const QSortFilterProxyModel *proxyModel)
{
    return reinterpret_cast<const QSqlTableModel*>(proxyModel->sourceModel());
}
