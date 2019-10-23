#include "projectstab.h"
#include "projectsdelegate.h"

#include <QLoggingCategory>
#include <QPushButton>

int ProjectsTab::openedWinCount = 0;

ProjectsTab::ProjectsTab(QWidget *parent)
    : CommonTab(parent), editCustomerFilter(new QLineEdit)
{
    editDelegate = new ProjectsDelegate;

    tableView->setWindowTitle("Projects");
    tableView->setItemDelegate(editDelegate);

    auto buttonProjectInfo = new QPushButton(tr("Project info"));
    buttonProjectInfo->setMaximumWidth(buttonWidthMax);
    buttonLayout->addWidget(buttonProjectInfo);

    auto labelFilter = new QLabel(tr("Filter by customer"));
    labelFilter->setMaximumWidth(buttonWidthMax);

    auto filterLayout = new QHBoxLayout;
    filterLayout->addWidget(labelFilter);
    filterLayout->addWidget(editCustomerFilter);

    tabLayout->addLayout(filterLayout);

    connect(buttonProjectInfo, SIGNAL(clicked()), this, SLOT(showProjectInfo()));
    connect(editCustomerFilter, SIGNAL(textEdited(const QString &)), this, SLOT(setProjectFilterByCustomer(const QString &)));
    connect(tableView, SIGNAL(highlightedRowChange(int)), editDelegate, SLOT(highlightedRowChanged(int)));
    connect(this, SIGNAL(setActive(bool)), buttonProjectInfo, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(setActive(bool)), editCustomerFilter, SLOT(setEnabled(bool)));
}

void ProjectsTab::showProjectInfo(void)
{
    auto selection = tableView->selectionModel();
    if (selection->hasSelection())
    {
        auto selectedRows = selection->selectedRows();
        for (auto& index : selectedRows)
        {
            if (openedWinCount >= maxProjectInfoWinNumber)
            {
                qDebug() << "Can't open more than" << openedWinCount << "windows at one time";
                break;
            }

            auto sourceIndex = getProxyModel()->mapToSource(index);
            auto record = getSourceModel()->record(sourceIndex.row());

            auto projectInfoWindow = new ProjectInfoWindow(ProjectInfo(record), this);
            connect(projectInfoWindow, SIGNAL(closed(ProjectInfoWindow*)), this, SLOT(hideProjectInfo(ProjectInfoWindow*)), Qt::QueuedConnection);
            connect(projectInfoWindow, SIGNAL(accepted(ProjectInfoWindow*)), this, SLOT(saveProjectInfo(ProjectInfoWindow*)), Qt::QueuedConnection);
            projectInfoWindow->show();
            openedWinCount++;
        }
    }
}

void ProjectsTab::saveProjectInfo(ProjectInfoWindow *window)
{
    auto updatedInfo = window->getUpdatedInfo();
    auto tableModel = getSourceModel();

    RowSelection selection(this, window->getProject());
    hideProjectInfo(window);

    const int row = 0;
    int column = 0;

    if (!tableModel->index(row, column).isValid())
    {
        qDebug() << "Record was removed earlier";
        return;
    }

    auto record = tableModel->record(row);
    ProjectInfo oldInfo(record);

    for (column = 0; column < tableModel->columnCount(); column++)
    {
        const auto oldValue = oldInfo[column];
        const auto newValue = updatedInfo[column];

        if (newValue == oldValue)
            continue;

        qDebug() << "Validating change:" << oldValue << "=>" << newValue;

        if (!editDelegate->validateValue(tableModel, record, column, newValue))
        {
            qDebug() << "Validation failed";
            continue;
        }

        record.setValue(column, newValue);
    }

    if (!tableModel->setRecord(row, record))
    {
        qWarning() << "SQL Record set failed";
    }
}

void ProjectsTab::hideProjectInfo(ProjectInfoWindow *window)
{
    qDebug() << "Project info window is closed";
    openedWinCount--;
    delete window;
}

void ProjectsTab::selectRowByKey(const QString &project)
{
    qDebug() << "select";
    auto tableModel = getSourceModel();
    tableModel->setFilter(QString("%1 = '%2'").arg(DatabaseMaintainer::projectsKeyName).arg(project));
    tableModel->filter();
}

void ProjectsTab::deselectRow(void)
{
    qDebug() << "deselect";
    getSourceModel()->setFilter("");
}

void ProjectsTab::setProjectFilterByCustomer(const QString &newText)
{
    getProxyModel()->setFilterKeyColumn(DatabaseMaintainer::ProjectsTableColumn::customer);
    getProxyModel()->setFilterRegExp(newText);
}

void ProjectsTab::setTableModel(QSqlTableModel *model)
{
    CommonTab::setTableModel(model);
    tableView->setColumnHidden(DatabaseMaintainer::description, true);
}
