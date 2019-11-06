#include <QApplication>

#include "databasemaintainer.h"
#include "logger.h"
#include "testdb.h"
#include "modelview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto args = QApplication::arguments();
    bool createTestDb = argc > 1 && args.at(1) == "test";

    startLogging(); //TODO: get directory/file from args

    if (createTestDb)
        TestDb db;

    DatabaseMaintainer dbMaintainer(TestDb::getTestDbNameDefault());
    ModelView mv(&dbMaintainer);

    mv.start();

    bool isOk = a.exec();

    return isOk;
}
