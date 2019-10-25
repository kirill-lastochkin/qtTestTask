#include "projectinfodelegate.h"

#include <QLoggingCategory>
#include <QLineEdit>

ProjectInfoDelegate::ProjectInfoDelegate(QWidget *parent)
    : ProjectsDelegate(parent)
{}

QWidget* ProjectInfoDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &,
                                           const QModelIndex &index) const
{
    auto editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    qDebug() << editor->text();
    //connect(editor, SIGNAL(returnPressed()), this, SLOT(finishEditing()));
    connect(editor, SIGNAL(editingFinished()), this, SLOT(finishEditing()));
    //connect(editor, SIGNAL(selectionChanged()), this, SLOT(finishEditing()));
    //connect(editor, SIGNAL(inputRejected()), this, SLOT(finishEditing()));
    editor->raise();
    return editor;
}

void ProjectInfoDelegate::finishEditing(void)
{
    emit editingFinished();
}
