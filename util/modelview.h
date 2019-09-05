#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "databasemaintainer.h"
#include "mainwindow.h"
#include "tab.h"
#include "customtablemodel.h"

class ModelView : public QWidget
{
    Q_OBJECT

public:
    ModelView(DatabaseMaintainer *dm);

    void start(void);

private:
    void configureModelCustomers();
    void configureModelProjects();

    SqlTableModelCustomers *modelCustomers;
    SqlTableModelProjects *modelProjects;
    MainWindow *view;
};

#endif // MODELVIEW_H
