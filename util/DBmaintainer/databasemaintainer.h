#ifndef DATABASEMAINTAINER_H
#define DATABASEMAINTAINER_H

#include "customerinfo.h"
#include "projectinfo.h"

#include <QSqlDatabase>

class DatabaseMaintainer
{
public:
    static const char * const customersTableName;
    static const char * const customersKeyName;
    static const char * const customersCountryName;
    static const char * const customersAddressName;

    static const char * const projectsTableName;
    static const char * const projectsKeyName;
    static const char * const projectsCustomerName;
    static const char * const projectsDescName;
    static const char * const projectsEndDateName;
    static const char * const projectsStartDateName;

    static const char * const dateFormat;

    static const int companyNameMaxLen;
    static const int countryNameMaxLen;
    static const int addressMaxLen;
    static const int projectNameMaxLen;
    static const int descriptionMaxLen;

    enum CustomersTableColumn
    {
        customersTableKey = 0,
        company = customersTableKey,
        country = 1,
        address = 2,
        customersTableColumnCount,
    };

    enum ProjectsTableColumn
    {
        projectsTableKey = 0,
        project = projectsTableKey,
        customer = 1,
        description = 2,
        startDate = 3,
        endDate = 4,
        projectsTableColumnCount,
    };

    DatabaseMaintainer();
    explicit DatabaseMaintainer(const QString &dbName);
    ~DatabaseMaintainer();

    bool connectDatabase(const QString &dbName);

    bool createCustomersTable(void);
    bool insertCustomer(const CustomerInfo &info);
    bool removeCustomer(const QString &company);

    bool createProjectsTable(void);
    bool insertProject(const ProjectInfo &info);
    bool removeProject(const QString &project);

    const QSqlDatabase &getDb(void) { return *db; }

private:
    enum class DatabaseState
    {
        NotConnected,
        Connected,
        ConnectionFailed,
        Ready,
        Failed,
    };

    const char * const connectionName = "connection_dflt";

    QSqlDatabase *db;
    DatabaseState state = DatabaseState::NotConnected;

    bool makeQuery(const QString &queryString);
};

typedef DatabaseMaintainer::ProjectsTableColumn ProjectsColumns;
typedef DatabaseMaintainer::CustomersTableColumn CustomersColumns;

#endif // DATABASEMAINTAINER_H
