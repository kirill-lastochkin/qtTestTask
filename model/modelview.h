#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "databasemaintainer.h"
#include "mainwindow.h"
#include "customtablemodels.h"
#include "projectinfo.h"
#include "projectinfowindow.h"

using MVPair = QPair<QSqlTableModel *, CommonTab *>;

class ModelView : public QWidget
{
    Q_OBJECT

public:
    explicit ModelView(DatabaseMaintainer *dm);

    void start(void);
    void setupModel(DatabaseMaintainer *dm);
    void setupTableView(void);
    void setupProjectView(const QString &project);

    ProjectsTab* projectsTab(void) { return mainWindow->getProjectsTab(); }
    CustomersTab* customersTab(void) { return mainWindow->getCustomersTab(); }

public slots:
    void removeEntries(const QString &table, QVector<int> &rows);
    void addEntry(const QString &table);
    void showProject(const QString &project);
    void closeProjectView(ProjectInfoWindow *window);
    void revertProjectChanges(ProjectInfoWindow *window);

private:
    inline MVPair tableModelView(const QString &table);
    void setProject(const ProjectInfo &info, const QString &key);

    SqlTableModelCustomers *modelCustomers;
    SqlTableModelProjects *modelProjects;
    MainWindow *mainWindow;

    int openedProjectInfoWinNumber = 0;
    const int maxProjectInfoWinNumber = 4;
};

#endif // MODELVIEW_H
