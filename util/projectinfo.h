#ifndef PROJECTSTRUCTS_H
#define PROJECTSTRUCTS_H

#include <QString>
#include <QDate>
#include <QSqlRecord>
#include <QVariant>

struct ProjectInfo
{
    QString project;
    QString customer;
    QString description;
    QDate start;
    QDate end;

    ProjectInfo() = default;
    explicit ProjectInfo(const QSqlRecord &record);
    ProjectInfo(const ProjectInfo &other);
    explicit ProjectInfo(const QString &argProject, const QString &argCustomer, const QString &argDesc,
                         const QDate &argStart, const QDate &argEnd);

    QVariant operator[](int index) const;
};

#endif // PROJECTSTRUCTS_H
