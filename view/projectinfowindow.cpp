#include "projectinfowindow.h"
#include "tab.h"
#include "mainwindow.h"

#include <QLoggingCategory>
#include <QDialogButtonBox>
#include <QKeyEvent>

ProjectInfoWindow::ProjectInfoWindow(const DatabaseMaintainer::ProjectInfo &info, QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
      editProject(new QLineEdit(info.project)), editCustomer(new QLineEdit(info.customer)),
      editStart(new QLineEdit(info.start.toString(DatabaseMaintainer::dateFormat))),
      editEnd(new QLineEdit(info.end.toString(DatabaseMaintainer::dateFormat))),
      editDesc(new QLineEdit(info.description))

{
    resize(MainWindow::windowWidthDefault, MainWindow::windowHeightDefault);
    setWindowTitle(tr("Project information"));

    auto labelProject = new QLabel(tr("Project name"));
    auto labelCustomer = new QLabel(tr("Customer name"));
    auto labelStart = new QLabel(tr("Project start date"));
    auto labelEnd = new QLabel(tr("Project end date"));
    auto labelDesc = new QLabel(tr("Project description"));

    editProject->setReadOnly(true);

    auto buttonBox = new QDialogButtonBox;
    buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Close);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(labelProject);
    mainLayout->addWidget(editProject);
    mainLayout->addWidget(labelCustomer);
    mainLayout->addWidget(editCustomer);
    mainLayout->addWidget(labelStart);
    mainLayout->addWidget(editStart);
    mainLayout->addWidget(labelEnd);
    mainLayout->addWidget(editEnd);
    mainLayout->addWidget(labelDesc);
    mainLayout->addWidget(editDesc);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    qDebug() << "Project info for" << info.project << "is opened";
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
    return editProject->text();
}

const QString ProjectInfoWindow::getCustomer(void)
{
    return editCustomer->text();
}
