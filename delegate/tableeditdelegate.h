#ifndef TABLEEDITDELEGATE_H
#define TABLEEDITDELEGATE_H

#include <QStyledItemDelegate>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

class TableEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TableEditDelegate(QWidget *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual bool validateValue(const QSqlTableModel *sourceModel, const QSqlRecord &oldRecord, int column, const QVariant &newValue) const = 0;

signals:
    void validationFailed(const QModelIndex &index) const;
    void validationSucceeded(const QModelIndex &index) const;

private slots:
    void highlightedRowChanged(int newRow);

protected:
    int highlightedRow = -1;
};

#endif // TABLEEDITDELEGATE_H
