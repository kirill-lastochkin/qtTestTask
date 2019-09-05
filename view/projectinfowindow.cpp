#include "projectinfowindow.h"
#include "tab.h"
#include "mainwindow.h"

#include <QLoggingCategory>
#include <QDialogButtonBox>
#include <QKeyEvent>

ProjectInfoWindow::ProjectInfoWindow(const DatabaseMaintainer::ProjectInfo &info, QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    //auto parentTab = reinterpret_cast<ProjectsTab*>(parent);
    //parentTab->
    resize(MainWindow::windowWidthDefault, MainWindow::windowHeightDefault);
    setWindowTitle(tr("Project information"));

    auto labelProject = new QLabel(tr("Project name"));
    auto labelCustomer = new QLabel(tr("Customer name"));
    auto labelStart = new QLabel(tr("Project start date"));
    auto labelEnd = new QLabel(tr("Project end date"));
    auto labelDesc = new QLabel(tr("Project description"));

    auto editProject = new QLineEdit(info.project);
    auto editCustomer = new QLineEdit(info.customer);
    auto editStart = new QLineEdit(info.start.toString(Qt::DateFormat::ISODate));
    auto editEnd = new QLineEdit(info.end.toString(Qt::DateFormat::ISODate));
    auto editDesc = new QTextEdit(info.description);

    auto buttonBox = new QDialogButtonBox;
    buttonBox->setStandardButtons(QDialogButtonBox::Apply);

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
}

ProjectInfoWindow::~ProjectInfoWindow()
{
}

void ProjectInfoWindow::closeEvent(QCloseEvent* event)
{
    emit closed(this);
}

void ProjectInfoWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->type()) {
    case QEvent::KeyPress:
        if (event->key() == Qt::Key_Escape)
            emit closed(this);
        break;
    default:
        break;
    }

    QDialog::keyPressEvent(event);
}
