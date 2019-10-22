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
    ProjectInfo(const QSqlRecord &record);
    ProjectInfo(const QString &argProject, const QString &argCustomer, const QString &argDesc,
                const QDate &argStart, const QDate &argEnd);

#if 0
    bool operator==(const ProjectInfo &other) const;
    bool fillSqlRecord(QSqlRecord &record);
#endif

    QVariant operator[](int index);
};

#endif // PROJECTSTRUCTS_H
