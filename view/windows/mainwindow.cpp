#include "mainwindow.h"
#include "timeindicator.h"

#include <QLoggingCategory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), customersTab(new CustomersTab),
      projectsTab(new ProjectsTab)
{
    setWindowTitle(tr("Qt Test Task"));
    resize(windowWidthDefault, windowHeightDefault);

    auto windowWidget = new QWidget;
    auto mainLayout = new QVBoxLayout;
    windowWidget->setLayout(mainLayout);
    setCentralWidget(windowWidget);

    auto tabWidget = new QTabWidget;
    tabWidget->addTab(customersTab, tr("Customers"));
    tabWidget->addTab(projectsTab, tr("Projects"));

    auto time = new TimeIndicator;

    mainLayout->addWidget(time);
    mainLayout->addWidget(tabWidget);
    mainLayout->setAlignment(time, Qt::AlignRight);
}
