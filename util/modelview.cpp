#include "modelview.h"

ModelView::ModelView(DatabaseMaintainer *dm)
{
    modelProjects = new SqlTableModelProjects(this, dm->getDb());
    modelCustomers = new SqlTableModelCustomers(this, dm->getDb());
    view = new MainWindow(this);

    view->getProjectsTab()->setTableModel(modelProjects);
    view->getCustomersTab()->setTableModel(reinterpret_cast<QSqlTableModel*>(modelCustomers));
}

void ModelView::start(void)
{
    view->show();
}
