#include "modelview.h"
#include "projectinfowindow.h"

#include <QPair>
#include <QLoggingCategory>

ModelView::ModelView(DatabaseMaintainer *dm)
{
    setupModel(dm);
    setupTableView();
}

void ModelView::setupModel(DatabaseMaintainer *dm)
{
    modelProjects = new SqlTableModelProjects(this, dm->getDb());
    modelCustomers = new SqlTableModelCustomers(this, dm->getDb());
}

void ModelView::setupTableView(void)
{
    mainWindow = new MainWindow(this);
    projectsTab()->setTableModel(modelProjects);
    customersTab()->setTableModel(reinterpret_cast<QSqlTableModel*>(modelCustomers));

    connect(projectsTab(), SIGNAL(addButtonPressed(const QString&)), this, SLOT(addEntry(const QString&)));
    connect(customersTab(), SIGNAL(addButtonPressed(const QString&)), this, SLOT(addEntry(const QString&)));

    connect(projectsTab(), SIGNAL(entriesRemoved(const QString&, QVector<int>&)), this, SLOT(removeEntries(const QString&, QVector<int>&)));
    connect(customersTab(), SIGNAL(entriesRemoved(const QString&, QVector<int>&)), this, SLOT(removeEntries(const QString&, QVector<int>&)));

    connect(projectsTab(), SIGNAL(showProjectPressed(const QString &)), this, SLOT(showProject(const QString &)));
}

void ModelView::start(void)
{
    mainWindow->show();
}

void ModelView::addEntry(const QString &table)
{
    auto [targetModel, tab] = tableModelView(table);
    if (targetModel != nullptr)
    {
        int newRow = targetModel->rowCount();
        targetModel->insertRow(newRow);
        tab->addRow(newRow);
    }
}

void ModelView::removeEntries(const QString &table, QVector<int> &rows)
{
    QSqlTableModel *targetModel = tableModelView(table).first;
    if (targetModel != nullptr)
    {
        for (auto row : rows)
            targetModel->removeRow(row);

        targetModel->select();
    }
}

inline MVPair ModelView::tableModelView(const QString &table)
{
    if (table == DatabaseMaintainer::projectsTableName)
        return MVPair(modelProjects, projectsTab());
    else if (table == DatabaseMaintainer::customersTableName)
        return  MVPair(modelCustomers, customersTab());
    else
        return MVPair(nullptr, nullptr);
}

void ModelView::showProject(const QString &project)
{
    if (openedProjectInfoWinNumber >= maxProjectInfoWinNumber)
    {
        qDebug() << "Can't open more than" << maxProjectInfoWinNumber << "projects at once";
        return;
    }

    setupProjectView(project);
    openedProjectInfoWinNumber++;
}

void ModelView::setupProjectView(const QString &project)
{
    auto projectInfoWindow = new ProjectInfoWindow(mainWindow);
    projectInfoWindow->setModel(modelProjects, project);
    projectInfoWindow->saveCurrentProjectInfo();

    connect(projectInfoWindow, SIGNAL(accepted(ProjectInfoWindow*)), this, SLOT(closeProjectView(ProjectInfoWindow*)), Qt::QueuedConnection);
    connect(projectInfoWindow, SIGNAL(canceled(ProjectInfoWindow*)), this, SLOT(revertProjectChanges(ProjectInfoWindow*)), Qt::QueuedConnection);

    projectInfoWindow->show();
}

void ModelView::closeProjectView(ProjectInfoWindow *window)
{
    openedProjectInfoWinNumber--;
    delete window;
}

void ModelView::revertProjectChanges(ProjectInfoWindow *window)
{
    setProject(window->savedProjectInfo(), window->key());
    closeProjectView(window);
}

void ModelView::setProject(const ProjectInfo &info, const QString &key)
{
    for (int row = 0; row < modelProjects->rowCount(); row++)
    {
        auto record = modelProjects->record(row);
        if (record.value(ProjectsColumns::projectsTableKey).toString() == key)
        {
            for (int column = 0; column < ProjectsColumns::projectsTableColumnCount; column++)
                modelProjects->setData(modelProjects->index(row, column), info[column]);
            return;
        }
    }

    qDebug() << "Row wasn't found (probably already removed)";
}
