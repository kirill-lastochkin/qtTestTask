#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTabWidget>
#include <QMainWindow>

#include "databasemaintainer.h"
#include "timeindicator.h"
#include "projectstab.h"
#include "customerstab.h"

#include <QLoggingCategory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    CustomersTab* getCustomersTab(void) const { return customersTab; }
    ProjectsTab* getProjectsTab(void) const { return projectsTab; }

    static const int windowWidthDefault;
    static const int windowHeightDefault;

private:
    CustomersTab *customersTab;
    ProjectsTab *projectsTab;
};

#endif // MAINWINDOW_H
