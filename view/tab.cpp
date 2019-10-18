#include "mainwindow.h"
#include "tab.h"
#include "tableeditdelegate.h"
#include "modelview.h"
#include "deleterecorddialog.h"

#include <QVBoxLayout>
#include <QLoggingCategory>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSqlRecord>
#include <QPushButton>
#include <QKeyEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QStackedWidget>

#include <memory>

int ProjectsTab::openedWinCount = 0;

CommonTab::CommonTab(QWidget *parent)
    : QWidget(parent), tableView(new TableView), emptyDbLabel(new QLabel),
      tabLayout(new QVBoxLayout), buttonLayout(new QHBoxLayout),
      spaceLayout(new QHBoxLayout)
{
    tableView->setModel(new QSortFilterProxyModel);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto pic = std::make_unique<QPixmap>(emptyDbImagePath); //new QPixmap(emptyDbImagePath);
    emptyDbLabel->setPixmap(pic->scaled(MainWindow::windowWidthDefault, MainWindow::windowHeightDefault));

    auto widgetStack = new QStackedWidget;
    widgetStack->addWidget(tableView);
    widgetStack->addWidget(emptyDbLabel);
    widgetStack->setCurrentWidget(tableView);

    auto buttonAdd = new QPushButton(tr("Add"));
    buttonAdd->setMaximumWidth(buttonWidthMax);
    auto buttonDel = new QPushButton(tr("Delete"));
    buttonDel->setMaximumWidth(buttonWidthMax);

    spaceLayout->addStretch();

    buttonLayout->addLayout(spaceLayout);
    buttonLayout->addWidget(buttonAdd);
    buttonLayout->addWidget(buttonDel);

    tabLayout->addWidget(widgetStack);
    tabLayout->addLayout(buttonLayout);

    setLayout(tabLayout);

    connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(buttonDel, SIGNAL(clicked()), this, SLOT(askDelRow()));

    connect(this, SIGNAL(setActive(bool)), buttonAdd, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(setActive(bool)), buttonDel, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(setActiveTableWidget(QWidget*)), widgetStack, SLOT(setCurrentWidget(QWidget*)));
}

void CommonTab::setTableModel(QSqlTableModel *model)
{
    getProxyModel()->setSourceModel(model);
    if (model->rowCount() == 0)
        setEmptyDbImage();
}

void CommonTab::addRow(void)
{
    auto tableModel = getProxyModel();
    int rowCount = tableModel->rowCount();
    tableModel->insertRow(rowCount);
    auto index = tableModel->index(rowCount, 0);

    connect(editDelegate, SIGNAL(validationFailed(const QModelIndex &)), this, SLOT(editTableItemFailed(const QModelIndex &)), Qt::QueuedConnection);
    connect(editDelegate, SIGNAL(validationSucceeded(const QModelIndex &)), this, SLOT(editTableItemSucceeded(const QModelIndex &)), Qt::QueuedConnection);

    tableView->scrollToBottom();
    tableView->edit(index);

    qDebug() << "Add new row in " << getSourceModel()->tableName();
}

void CommonTab::editTableItemFailed(const QModelIndex &index)
{
    tableView->edit(index);
}

void CommonTab::editTableItemSucceeded(const QModelIndex &index)
{
    auto tableModel = getProxyModel();
    auto nextIndex = tableModel->index(index.row(), index.column() + 1);
    if (tableView->isColumnHidden(nextIndex.column()))
        nextIndex = tableModel->index(nextIndex.row(), nextIndex.column() + 1);

    if (nextIndex.isValid())
    {
        tableView->edit(nextIndex);
    }
    else
    {
        disconnect(editDelegate, SIGNAL(validationFailed(const QModelIndex &)), this, SLOT(editTableItemFailed(const QModelIndex &)));
        disconnect(editDelegate, SIGNAL(validationSucceeded(const QModelIndex &)), this, SLOT(editTableItemSucceeded(const QModelIndex &)));
    }

}

void CommonTab::askDelRow(void)
{
    auto selectedRows = tableView->selectionModel();
    if (!selectedRows->hasSelection())
        return;

    auto dialog = std::make_unique<DeleteRecordDialog>(this);
    if (dialog->exec() == QDialog::Accepted)
        delRow();
}

void CommonTab::delRow(void)
{
    auto tableModel = getSourceModel();
    int removedRowsCount = 0;

    auto selectedRows = tableView->selectionModel();
    if (selectedRows->hasSelection())
    {
        auto indexes = selectedRows->selectedIndexes();
        int row= -1;
        for (auto& index : indexes)
        {
            if (row == index.row())
                continue;

            row = index.row();
            getProxyModel()->removeRow(row);
            removedRowsCount++;
        }

    }

    tableModel->select();
    tableView->repaint();

    qDebug() << removedRowsCount << "rows are removed from" << tableModel->tableName();

    if (tableModel->rowCount() == 0)
    {
        qDebug() << "Table " << tableModel->tableName() << "is empty";
        setEmptyDbImage();
    }
}

void CommonTab::setEmptyDbImage(void)
{
    emit setActive(false);
    emit setActiveTableWidget(emptyDbLabel);
}

CustomersTab::CustomersTab(QWidget *parent)
    : CommonTab(parent)
{
    editDelegate = new CustomerTableEditDelegate;

    tableView->setWindowTitle("Customers");
    tableView->setItemDelegate(editDelegate);

    connect(tableView, SIGNAL(highlightedRowChange(int)), editDelegate, SLOT(highlightedRowChanged(int)));
}

ProjectsTab::ProjectsTab(QWidget *parent)
    : CommonTab(parent), editCustomerFilter(new QLineEdit)
{
    editDelegate = new ProjectTableEditDelegate;

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
    auto selectedRows = tableView->selectionModel();
    if (selectedRows->hasSelection())
    {
        auto indexes = selectedRows->selectedIndexes();
        int row = -1;
        for (auto& index : indexes)
        {
            if (row == index.row())
                continue;

            if (openedWinCount >= maxProjectInfoWinNumber)
            {
                qDebug() << "Can't open more than" << openedWinCount << "windows at one time";
                break;
            }

            row = index.row();

            auto sourceIndex = getProxyModel()->mapToSource(index);
            auto record = getSourceModel()->record(sourceIndex.row());

            DatabaseMaintainer::ProjectInfo info{ record.value(0).toString(),
                                                  record.value(1).toString(),
                                                  record.value(2).toString(),
                                                  record.value(3).toDate(),
                                                  record.value(4).toDate()};

            auto projectInfoWindow = new ProjectInfoWindow(info, this);
            connect(projectInfoWindow, SIGNAL(closed(ProjectInfoWindow*)), this, SLOT(hideProjectInfo(ProjectInfoWindow*)), Qt::QueuedConnection);
            connect(projectInfoWindow, SIGNAL(accepted(ProjectInfoWindow*)), this, SLOT(saveProjectInfo(ProjectInfoWindow*)), Qt::QueuedConnection);
            projectInfoWindow->show();
            openedWinCount++;

        }
    }
}

void ProjectsTab::hideProjectInfo(ProjectInfoWindow *window)
{
    qDebug() << "Project info window is closed";
    openedWinCount--;
    delete window;
}

void ProjectsTab::saveProjectInfo(ProjectInfoWindow *window)
{
    qDebug() << "Project info window provides new data";

    auto tableModel = getSourceModel();
    tableModel->setFilter(QString("%1 = '%2'").arg(DatabaseMaintainer::projectsKeyName).arg(window->getProject()));
    tableModel->filter();

    //auto ttt = getProxyModel();

    for (int col = 0; col < tableModel->columnCount(); col++)
    {
        auto index = tableModel->index(0, col);
        auto oldValue = tableModel->data(index).toString();
        QLineEdit *editor = nullptr;

        switch (col)
        {
        case DatabaseMaintainer::ProjectsTableColumn::project:
            editor = window->editProject;
            break;
        case DatabaseMaintainer::ProjectsTableColumn::customer:
            editor = window->editCustomer;
            break;
        case DatabaseMaintainer::ProjectsTableColumn::description:
            editor = window->editDesc;
            break;
        case DatabaseMaintainer::ProjectsTableColumn::startDate:
            editor = window->editStart;
            break;
        case DatabaseMaintainer::ProjectsTableColumn::endDate:
            editor = window->editEnd;
            break;
        default:
            qCritical() << "Unrecognized column:" << col;
            goto exit;
        }

        if (editor->text() == oldValue)
            continue;

        qDebug() << oldValue << "=>" << editor->text();
        //editDelegate->setModelData(editor, tableModel, index);
    }

exit:
    tableModel->setFilter("");

    hideProjectInfo(window);
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
