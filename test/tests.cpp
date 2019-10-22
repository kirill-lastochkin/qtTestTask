#include <QLoggingCategory>

#include "databasemaintainer.h"
#include "tests.h"

TestDb::TestDb(const char * name)
    : dm(name)
{
    if (!dm.createCustomersTable())
        return;
    if (!dm.createProjectsTable())
        return;

    for (int i = 0; i < testProjectsCount; i++)
    {
        QString projectName("Aveine" + QString::number(i));
        QString customerName("Noveo" + QString::number(i + 5));
        if (!dm.insertProject(ProjectInfo{projectName, customerName, "Late", QDate(2019, 9, 1), QDate(2019, 9, 30)}))
            return;
    }

    for (int i = 0; i < testCustomersCount; i++)
    {
        QString customerName("Noveo" + QString::number(i));
        if (!dm.insertCustomer(CustomerInfo{customerName, "Russia", "Akadem"}))
            return;
    }

    status = TestStatus::Succeded;
}

TestDb::TestDb()
    : TestDb(testDbNameDefault)
{}
