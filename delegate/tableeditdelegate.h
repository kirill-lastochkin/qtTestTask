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
    TableEditDelegate(QWidget *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void validationFailed(const QModelIndex &index) const;
    void validationSucceeded(const QModelIndex &index) const;

private slots:
    void highlightedRowChanged(int newRow);

protected:
    virtual bool validateValue(const QModelIndex &index, const QString &newValue) const = 0;
    virtual const QString getKeyName(const QModelIndex &index) const = 0;

    int highlightedRow = -1;
};

class CustomerTableEditDelegate : public TableEditDelegate
{
    Q_OBJECT

public:
    CustomerTableEditDelegate(QWidget *parent = nullptr)
        : TableEditDelegate(parent) {}

private:
    bool validateValue(const QModelIndex &index, const QString &newValue) const override;
    const QString getKeyName(const QModelIndex &index) const override;

    bool validateCompany(const QString &newValue, const QSqlTableModel* model, int currentRow) const;
    bool validateCountry(const QString &newValue) const;
    bool validateAddress(const QString &newValue) const;
};

class ProjectTableEditDelegate : public TableEditDelegate
{
    Q_OBJECT

public:
    ProjectTableEditDelegate(QWidget *parent = nullptr)
        : TableEditDelegate(parent) {}

private:
    bool validateValue(const QModelIndex &index, const QString &newValue) const override;
    const QString getKeyName(const QModelIndex &index) const override;

    bool validateProject(const QString &newValue, const QSqlTableModel* model, int currentRow) const;
    bool validateCustomer(const QString &newValue) const;
    bool validateDescription(const QString &newValue) const;
    bool validateStartDate(const QDate &newValue, const QSqlRecord &record) const;
    bool validateEndDate(const QDate &newValue, const QSqlRecord &record) const;
};

inline QSortFilterProxyModel* getProxyModel(QAbstractItemModel *model);
inline const QSortFilterProxyModel* getProxyModel(const QAbstractItemModel *model);
inline QSqlTableModel* getProxySourceSqlTableModel(QSortFilterProxyModel *proxyModel);
inline const QSqlTableModel* getProxySourceSqlTableModel(const QSortFilterProxyModel *proxyModel);

#endif // TABLEEDITDELEGATE_H
