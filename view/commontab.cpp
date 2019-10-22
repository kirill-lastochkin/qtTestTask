#include "mainwindow.h"
#include "commontab.h"
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
