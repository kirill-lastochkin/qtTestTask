#include "databasemaintainer.h"
const char * const DatabaseMaintainer::projectsTableName = "projects";
const char * const DatabaseMaintainer::projectsKeyName = "project";
const char * const DatabaseMaintainer::projectsCustomerName = "customer";
const char * const DatabaseMaintainer::projectsDescName = "description";
const char * const DatabaseMaintainer::projectsEndDateName = "end";
const char * const DatabaseMaintainer::projectsStartDateName = "start";

const char * const DatabaseMaintainer::customersTableName = "customers";
const char * const DatabaseMaintainer::customersKeyName = "company";
const char * const DatabaseMaintainer::customersCountryName = "country";
const char * const DatabaseMaintainer::customersAddressName = "address";

const char * const DatabaseMaintainer::dateFormat = "yyyy-MM-dd";

const int DatabaseMaintainer::companyNameMaxLen = 30;
const int DatabaseMaintainer::countryNameMaxLen = 50;
const int DatabaseMaintainer::addressMaxLen = 80;
const int DatabaseMaintainer::projectNameMaxLen = 20;
const int DatabaseMaintainer::descriptionMaxLen = 120;

#include "mainwindow.h"
const int MainWindow::windowWidthDefault = 500;
const int MainWindow::windowHeightDefault = 330;

#include "timeindicator.h"
const char * const TimeIndicator::timeDefaultMsg = "No connection with time server";
const char * const TimeIndicator::timeServerUrl = "http://worldtimeapi.org/api/timezone/Asia/Novosibirsk";
const int TimeIndicator::serverSyncPeriodSecs = 60;

#include "tab.h"
const int CommonTab::buttonWidthMax = 100;
const char * const CommonTab::emptyDbImagePath = ":/image/empty.jpg";

#include "tests.h"
const char * TestDb::testDbNameDefault  = "c:/test_zone/testdb.db3";
