#include "projectstab.h"
#include "projectsdelegate.h"

#include <QLoggingCategory>
#include <QPushButton>

ProjectsTab::ProjectsTab(QWidget *parent)
    : CommonTab(parent)
{
    editDelegate = new ProjectsDelegate;

    tableView->setWindowTitle("Projects");
    tableView->setItemDelegate(editDelegate);

    auto buttonProjectInfo = new QPushButton(tr("Project info"));
    buttonProjectInfo->setMaximumWidth(buttonWidthMax);
    buttonLayout->addWidget(buttonProjectInfo);

    auto labelFilter = new QLabel(tr("Filter by customer"));
    labelFilter->setMaximumWidth(buttonWidthMax);

    auto editCustomerFilter = new QLineEdit;
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

void ProjectsTab::setTableModel(QSqlTableModel *model)
{
    CommonTab::setTableModel(model);
    tableView->setColumnHidden(DatabaseMaintainer::description, true);
}

void ProjectsTab::showProjectInfo(void)
{
    auto selection = tableView->selectionModel();
    if (!selection->hasSelection())
        return;

    for (auto& index : selection->selectedRows())
    {
        auto project = getSqlRecord(index, tableView).value(ProjectsColumns::projectsTableKey).toString();
        emit showProjectPressed(project);
    }
}

void ProjectsTab::setProjectFilterByCustomer(const QString &newText)
{
    proxyModel()->setFilterRegExp(newText);
    proxyModel()->setFilterKeyColumn(ProjectsColumns::customer);
    emit addingAllowed(newText.isEmpty());
}
