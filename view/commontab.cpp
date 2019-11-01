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
      tabLayout(new QVBoxLayout), buttonLayout(new QHBoxLayout)
{
    tableView->setModel(new QSortFilterProxyModel);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto pic = std::make_unique<QPixmap>(emptyDbImagePath);
    emptyDbLabel->setPixmap(pic->scaled(MainWindow::windowWidthDefault, MainWindow::windowHeightDefault));

    auto widgetStack = new QStackedWidget;
    widgetStack->addWidget(tableView);
    widgetStack->addWidget(emptyDbLabel);
    widgetStack->setCurrentWidget(tableView);

    auto buttonAdd = new QPushButton(tr("Add"));
    buttonAdd->setMaximumWidth(buttonWidthMax);
    auto buttonDel = new QPushButton(tr("Delete"));
    buttonDel->setMaximumWidth(buttonWidthMax);

    auto spaceLayout(new QHBoxLayout);
    spaceLayout->addStretch();

    buttonLayout->addLayout(spaceLayout);
    buttonLayout->addWidget(buttonAdd);
    buttonLayout->addWidget(buttonDel);

    tabLayout->addWidget(widgetStack);
    tabLayout->addLayout(buttonLayout);

    setLayout(tabLayout);

    //connect(buttonAdd, &QPushButton::clicked, [this]() { emit this->addRowPressed(this->sourceModel()->tableName()); });
    connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(buttonDel, SIGNAL(clicked()), this, SLOT(delRow()));

    connect(this, SIGNAL(setActive(bool)), buttonAdd, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(setActive(bool)), buttonDel, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(setActiveTableWidget(QWidget*)), widgetStack, SLOT(setCurrentWidget(QWidget*)));
}

void CommonTab::addRow(void)
{
    int newRow = proxyModel()->rowCount();
    proxyModel()->insertRow(newRow);
    tableView->scrollToBottom();

    connect(editDelegate, SIGNAL(validationFailed(const QModelIndex &)), this, SLOT(editTableItemFailed(const QModelIndex &)), Qt::QueuedConnection);
    connect(editDelegate, SIGNAL(validationSucceeded(const QModelIndex &)), this, SLOT(editTableItemSucceeded(const QModelIndex &)), Qt::QueuedConnection);

    int firstColumn = 0;
    tableView->edit(proxyModel()->index(newRow, firstColumn));
}

void CommonTab::delRow(void)
{
    auto selection = tableView->selectionModel();
    if (!selection->hasSelection())
        return;

    if (!confirmDeletion())
        return;

    QVector<int> rows;

    auto selectedRows = selection->selectedRows();
    for (auto &rowIndex : selectedRows)
    {
        auto sourceIndex = proxyModel()->mapToSource(rowIndex);
        rows.push_front(sourceIndex.row());
    }

    emit entryRemoved(sourceModel()->tableName(), rows);

    checkEmptyModel(sourceModel());
    repaint();

    qDebug() << selectedRows.count() << "rows were removed from" << sourceModel()->tableName();
}

bool CommonTab::confirmDeletion(void)
{
    auto dialog = std::make_unique<DeleteRecordDialog>(this);
    return (dialog->exec() == QDialog::Accepted);
}

void CommonTab::setTableModel(QSqlTableModel *model)
{
    proxyModel()->setSourceModel(model);
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
    return proxyModel()->index(index.row(), index.column() + 1);
}
