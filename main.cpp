#include <QApplication>

#include "databasemaintainer.h"
#include "logger.h"
#include "testdb.h"
#include "modelview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Logger::getLogger().initLogger("logs.txt", "logs"); //redirect logs to file (TODO file/folder name from args)
    //TestDb db; //create test DB (optional)
    DatabaseMaintainer dbMaintainer("c:/test_zone/testdb.db3"); //connect to DB (TODO DB name from args)
    ModelView mv(&dbMaintainer);

    mv.start();

    bool isOk = a.exec();

    return isOk;
}
