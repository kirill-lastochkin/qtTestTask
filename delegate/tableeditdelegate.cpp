#include "tableeditdelegate.h"
#include "databasemaintainer.h"

#include <QLoggingCategory>
#include <QLineEdit>
#include <QPainter>

void TableEditDelegate::setModelData(QWidget *editor,
                                     QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    auto lineEditor = reinterpret_cast<QLineEdit*>(editor);
    auto proxyModel = reinterpret_cast<QSortFilterProxyModel*>(model);
    auto sourceModel = reinterpret_cast<QSqlTableModel*>(proxyModel->sourceModel());

    qDebug() << "Validating new value in table" << sourceModel->tableName();

    if (!validateValue(sourceModel, sourceModel->record(index.row()), index.column(), lineEditor->text()))
    {
        qDebug() << "Validation failed";
        emit validationFailed(index);
        return;
    }

    emit validationSucceeded(index);

    QStyledItemDelegate::setModelData(editor, model, index);
}

void TableEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int row = index.row();
    Qt::GlobalColor color;

    if (row == highlightedRow)
        color = Qt::red;
    else if (row % 2 == 1)
        color = Qt::lightGray;
    else
        color = Qt::white;

    painter->fillRect(option.rect, color);

    QStyledItemDelegate::paint(painter, option, index);
}

void TableEditDelegate::highlightedRowChanged(int newRow)
{
    highlightedRow = newRow;
}
