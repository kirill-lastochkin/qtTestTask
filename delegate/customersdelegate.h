#ifndef CUSTOMERSDELEGATE_H
#define CUSTOMERSDELEGATE_H

#include "tableeditdelegate.h"

class CustomersDelegate : public TableEditDelegate
{
    Q_OBJECT

public:
    explicit CustomersDelegate(QWidget *parent = nullptr)
        : TableEditDelegate(parent) {}

private:
    bool validateValue(const QSqlTableModel *sourceModel, const QSqlRecord&, int column, const QVariant &newValue) const override;

    bool validateCompany(const QString &newValue, const QSqlTableModel* model) const;
    bool validateCountry(const QString &newValue) const;
    bool validateAddress(const QString &newValue) const;
};

#endif // CUSTOMERSDELEGATE_H
