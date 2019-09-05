#ifndef TESTS_H
#define TESTS_H

#include "databasemaintainer.h"

class TestDb
{
public:
    enum class TestStatus { Succeded, Failed };

    TestDb();
    explicit TestDb(const char * name);

     const char * getTestDbNameDefault(void)
     {
         return testDbNameDefault;
     }

private:
    static const char * testDbNameDefault;
    DatabaseMaintainer dm;
    TestStatus status = TestStatus::Failed;
    int testProjectsCount = 40;
    int testCustomersCount = 20;
};

#endif // TESTS_H
