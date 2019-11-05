#include "projectinfowindow.h"
#include "commontab.h"
#include "mainwindow.h"
#include "projectinfoview.h"

#include <QLoggingCategory>
#include <QDialogButtonBox>
#include <QKeyEvent>

ProjectInfoWindow::ProjectInfoWindow(QWidget *parent)
    : QDialog(parent), view(new ProjectInfoView)
{
    resize(windowWidthDefault, windowHeightDefault);
    setWindowTitle(tr("Project information"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(view, SIGNAL(keyValueChanged(const QString&)), this, SLOT(changeKeyValue(const QString&)));
}

void ProjectInfoWindow::setModel(QSqlTableModel *model, const QString &key)
{
    proxyModel()->setSourceModel(model);
    proxyModel()->setFilterKeyColumn(ProjectsColumns::projectsTableKey);
    proxyModel()->setFilterRegExp(key + "$");
    view->updateText();
}

void ProjectInfoWindow::saveCurrentProjectInfo(void)
{
    oldProjectInfo = ProjectInfo(getSqlRecord(view));
}

void ProjectInfoWindow::accept()
{
    QDialog::accept();
    emit accepted(this);
}

void ProjectInfoWindow::reject()
{
    QDialog::reject();
    emit canceled(this);
}

void ProjectInfoWindow::closeEvent(QCloseEvent*)
{
    emit accepted(this);
}
