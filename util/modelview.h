#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "databasemaintainer.h"
#include "mainwindow.h"
#include "commontab.h"
#include "customtablemodel.h"
#include "projectinfo.h"

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

    ProjectsTab* projectsTab(void) { return view->getProjectsTab(); }
    CustomersTab* customersTab(void) { return view->getCustomersTab(); }

public slots:
    void showProject(const QString &project);
    void closeProjectWindow(ProjectInfoWindow *window);
    void revertProjectChanges(ProjectInfoWindow *window);
    void setProject(const ProjectInfo &info, const QString &key);
    void removeEntry(const QString &table, QVector<int> &rows);
    void addEntry(const QString &table);

private:
    inline MVPair getTableModelView(const QString &table);

    SqlTableModelCustomers *modelCustomers;
    SqlTableModelProjects *modelProjects;
    MainWindow *view;
};

#endif // MODELVIEW_H
