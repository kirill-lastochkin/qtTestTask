#include "modelview.h"
#include "projectinfowindow.h"

#include <QPair>

ModelView::ModelView(DatabaseMaintainer *dm)
{
    setupModel(dm);
    setupTableView();
}

void ModelView::start(void)
{
    view->show();
}

void ModelView::setupModel(DatabaseMaintainer *dm)
{
    modelProjects = new SqlTableModelProjects(this, dm->getDb());
    modelCustomers = new SqlTableModelCustomers(this, dm->getDb());
}

void ModelView::setupTableView(void)
{
    view = new MainWindow(this);
    projectsTab()->setTableModel(modelProjects);
    customersTab()->setTableModel(reinterpret_cast<QSqlTableModel*>(modelCustomers));

    connect(projectsTab(), SIGNAL(showProjectPressed(const QString &)), this, SLOT(showProject(const QString &)));

    connect(projectsTab(), SIGNAL(entryRemoved(const QString&, QVector<int>&)), this, SLOT(removeEntry(const QString&, QVector<int>&)));
    connect(customersTab(), SIGNAL(entryRemoved(const QString&, QVector<int>&)), this, SLOT(removeEntry(const QString&, QVector<int>&)));

    connect(projectsTab(), SIGNAL(addRowPressed(const QString&)), this, SLOT(addEntry(const QString&)));
    connect(customersTab(), SIGNAL(addRowPressed(const QString&)), this, SLOT(addEntry(const QString&)));
}

void ModelView::showProject(const QString &project)
{
    setupProjectView(project);
}

void ModelView::setupProjectView(const QString &project)
{
    auto projectInfoWindow = new ProjectInfoWindow(view);
    projectInfoWindow->setModel(modelProjects, project);
    projectInfoWindow->saveCurrentProjectInfo();

    connect(projectInfoWindow, SIGNAL(accepted(ProjectInfoWindow*)), this, SLOT(closeProjectWindow(ProjectInfoWindow*)), Qt::QueuedConnection);
    connect(projectInfoWindow, SIGNAL(canceled(ProjectInfoWindow*)), this, SLOT(revertProjectChanges(ProjectInfoWindow*)), Qt::QueuedConnection);

    projectInfoWindow->show();
}

void ModelView::closeProjectWindow(ProjectInfoWindow *window)
{
    qDebug() << "Project info window is closed";
    projectsTab()->projectWindowClose();
    delete window;
}

void ModelView::revertProjectChanges(ProjectInfoWindow *window)
{
    setProject(window->savedProjectInfo(), window->key());
    closeProjectWindow(window);
}

void ModelView::setProject(const ProjectInfo &info, const QString &key)
{
    qDebug() << "Set project" << key;
    int row, maxRow = modelProjects->rowCount();
    for (row = 0; row < maxRow; row++)
    {
        auto record = modelProjects->record(row);
        if (record.value(DatabaseMaintainer::ProjectsTableColumn::project).toString() == key)
            break;
    }

    if (row == maxRow)
    {
        qDebug() << "Row wasn't found (probably already removed)";
        return;
    }

    for (int column = 0; column < DatabaseMaintainer::projectsTableColumnCount; column++)
        modelProjects->setData(modelProjects->index(row, column), info[column]);
}

inline MVPair ModelView::getTableModelView(const QString &table)
{
    if (table == DatabaseMaintainer::projectsTableName)
        return MVPair(modelProjects, projectsTab());
    else if (table == DatabaseMaintainer::customersTableName)
        return  MVPair(modelCustomers, customersTab());
    else
        return MVPair(nullptr, nullptr);
}

void ModelView::removeEntry(const QString &table, QVector<int> &rows)
{
    QSqlTableModel *targetModel = getTableModelView(table).first;
    if (targetModel != nullptr)
    {
        for (auto row : rows)
            targetModel->removeRow(row);

        targetModel->select();
    }
}

void ModelView::addEntry(const QString &table)
{
    auto [targetModel, tab] = getTableModelView(table);
    if (targetModel != nullptr)
    {
        qDebug() << "Add new row in" << table;
        targetModel->insertRow(0);
        //tab->addRow(0);
    }
}
