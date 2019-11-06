#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "projectstab.h"
#include "customerstab.h"

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
