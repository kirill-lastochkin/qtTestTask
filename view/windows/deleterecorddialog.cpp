#include "deleterecorddialog.h"
#include <QLabel>
#include <QDialogButtonBox>
#include <QBoxLayout>

DeleteRecordDialog::DeleteRecordDialog(QWidget *parent)
    : QDialog (parent)
{
    setWindowTitle(tr("Removing row"));

    auto label = new QLabel(tr("Are you sure?"));
    auto buttonBox = new QDialogButtonBox;
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(buttonBox);
    setLayout(layout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
