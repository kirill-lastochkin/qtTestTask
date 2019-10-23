#include "projectinfowindow.h"
#include "commontab.h"
#include "mainwindow.h"

#include <QLoggingCategory>
#include <QDialogButtonBox>
#include <QKeyEvent>

ProjectInfoWindow::ProjectInfoWindow(const QSqlTableModel *sourceModel, int row, QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
      projectInfo(sourceModel->record(row)), oldProjectName(projectInfo.project), model(sourceModel)
{
    resize(MainWindow::windowWidthDefault, MainWindow::windowHeightDefault);
    setWindowTitle(tr("Project information"));

    auto mainLayout = new QVBoxLayout;

    const char *headers[] = { "Project name", "Customer", "Description", "Start date", "End date"};
    for (int column = 0; column < DatabaseMaintainer::projectsTableColumnCount; column++)
    {
        auto label = new QLabel(tr(headers[column]));
        data[column] = new QLineEdit(projectInfo[column].toString());
        mainLayout->addWidget(label);
        mainLayout->addWidget(data[column]);
    }

    auto buttonBox = new QDialogButtonBox;
    buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Close);

    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    qDebug() << "Project info for" << projectInfo.project << "is opened";
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
