#ifndef PROJECTSDELEGATE_H
#define PROJECTSDELEGATE_H

#include "tableeditdelegate.h"

class ProjectsDelegate : public TableEditDelegate
{
    Q_OBJECT

public:
    ProjectsDelegate(QWidget *parent = nullptr)
        : TableEditDelegate(parent) {}

private:
    bool validateValue(const QSqlTableModel *sourceModel, const QSqlRecord &oldRecord, int column, const QVariant &newValue) const override;
    bool validateProject(const QString &newValue, const QSqlTableModel* model) const;
    bool validateCustomer(const QString &newValue) const;
    bool validateDescription(const QString &newValue) const;
    bool validateStartDate(const QDate &newValue, const QSqlRecord &record) const;
    bool validateEndDate(const QDate &newValue, const QSqlRecord &record) const;
};

#endif // PROJECTSDELEGATE_H
