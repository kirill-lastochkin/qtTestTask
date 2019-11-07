#include "customerstab.h"
#include "customersdelegate.h"

CustomersTab::CustomersTab(QWidget *parent)
    : CommonTab(parent)
{
    editDelegate = new CustomersDelegate;

    tableView->setWindowTitle("Customers");
    tableView->setItemDelegate(editDelegate);

    connect(tableView, SIGNAL(highlightedRowChange(int)), editDelegate, SLOT(highlightedRowChanged(int)));
}

void CustomersTab::addRow(int newRow)
{
    auto record = getSqlRecord(proxyModel()->index(newRow - 1, 0), tableView);
    sourceModel()->setRecord(newRow, record);

    CommonTab::addRow(newRow);
}
