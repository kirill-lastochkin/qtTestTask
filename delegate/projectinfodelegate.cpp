#include "projectinfodelegate.h"

#include <QLineEdit>

QWidget* ProjectInfoDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &,
                                           const QModelIndex &index) const
{
    auto editor = new QLineEdit(index.data(Qt::DisplayRole).toString(), parent);
    connect(editor, &QLineEdit::editingFinished, this, &ProjectInfoDelegate::editingFinished);
    return editor;
}
