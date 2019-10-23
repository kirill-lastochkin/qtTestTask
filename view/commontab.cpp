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
    connect(buttonDel, SIGNAL(clicked()), this, SLOT(delRow()));

    connect(this, SIGNAL(setActive(bool)), buttonAdd, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(setActive(bool)), buttonDel, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(setActiveTableWidget(QWidget*)), widgetStack, SLOT(setCurrentWidget(QWidget*)));
}

void CommonTab::addRow(void)
{
    qDebug() << "Add new row in " << getSourceModel()->tableName();

    auto tableModel = getProxyModel();
    int newRowNumber = tableModel->rowCount();

    tableModel->insertRow(newRowNumber);
    tableView->scrollToBottom();

    connect(editDelegate, SIGNAL(validationFailed(const QModelIndex &)), this, SLOT(editTableItemFailed(const QModelIndex &)), Qt::QueuedConnection);
    connect(editDelegate, SIGNAL(validationSucceeded(const QModelIndex &)), this, SLOT(editTableItemSucceeded(const QModelIndex &)), Qt::QueuedConnection);

    int firstColumn = 0;
    tableView->edit(tableModel->index(newRowNumber, firstColumn));
}

void CommonTab::delRow(void)
{
    auto selection = tableView->selectionModel();
    if (!selection->hasSelection())
        return;

    if (!confirmDeletion())
        return;

    auto selectedRows = selection->selectedRows();
    for (auto &rowIndex : selectedRows)
        getProxyModel()->removeRow(rowIndex.row());

    auto tableModel = getSourceModel();
    tableModel->select();
    tableView->repaint();

    qDebug() << selectedRows.count() << "rows were removed from" << tableModel->tableName();

    checkEmptyModel(tableModel);
}

bool CommonTab::confirmDeletion(void)
{
    auto dialog = std::make_unique<DeleteRecordDialog>(this);
    return (dialog->exec() == QDialog::Accepted);
}

void CommonTab::setTableModel(QSqlTableModel *model)
{
    getProxyModel()->setSourceModel(model);
    checkEmptyModel(model);
}

inline void CommonTab::checkEmptyModel(const QSqlTableModel *model)
{
    if (model->rowCount() == 0)
    {
        qDebug() << "Table " << model->tableName() << "is empty";
        setEmptyDbImage();
    }
}

void CommonTab::setEmptyDbImage(void)
{
    emit setActive(false);
    emit setActiveTableWidget(emptyDbLabel);
}

void CommonTab::editTableItemSucceeded(const QModelIndex &index)
{
    auto nextIndex = getNextIndex(index);
    if (tableView->isColumnHidden(nextIndex.column()))
        nextIndex = getNextIndex(nextIndex);

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

void CommonTab::editTableItemFailed(const QModelIndex &index)
{
    tableView->edit(index);
}

inline QModelIndex CommonTab::getNextIndex(const QModelIndex &index)
{
    return getProxyModel()->index(index.row(), index.column() + 1);
}
