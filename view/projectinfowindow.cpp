#include "projectinfowindow.h"
#include "commontab.h"
#include "mainwindow.h"
#include "projectinfoview.h"

#include <QLoggingCategory>
#include <QDialogButtonBox>
#include <QKeyEvent>

ProjectInfoWindow::ProjectInfoWindow(QSqlTableModel *sourceModel, int row, QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
      projectInfo(sourceModel->record(row)), oldProjectName(projectInfo.project), model(sourceModel)
{
    resize(MainWindow::windowWidthDefault, MainWindow::windowHeightDefault);
    setWindowTitle(tr("Project information"));

    auto view = new ProjectInfoView(sourceModel, oldProjectName);
    auto buttonBox = new QDialogButtonBox;
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ProjectInfoWindow::accept()
{
    QDialog::accept();
    emit accepted(this);
}

void ProjectInfoWindow::closeEvent(QCloseEvent*)
{
    emit closed(this);
}

void ProjectInfoWindow::reject()
{
    emit closed(this);
    QDialog::reject();
}

const QString ProjectInfoWindow::getProject(void)
{
    return oldProjectName;
}

const ProjectInfo ProjectInfoWindow::getUpdatedInfo(void)
{
    return projectInfo;
}
