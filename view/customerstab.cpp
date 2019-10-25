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

void CustomersTab::selectRowByKey(const QString &customer)
{
    sourceModel()->setFilter(QString("%1 = '%2'").arg(DatabaseMaintainer::customersKeyName).arg(customer));
    sourceModel()->filter();
}

void CustomersTab::deselectRow(void)
{
    sourceModel()->setFilter("");
}
