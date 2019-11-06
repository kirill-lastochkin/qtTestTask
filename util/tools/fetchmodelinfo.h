#ifndef FETCHMODELINFO_H
#define FETCHMODELINFO_H

#include <QSortFilterProxyModel>
#include <QAbstractItemView>
#include <QSqlTableModel>
#include <QSqlRecord>

inline QSortFilterProxyModel* viewProxyModel(QAbstractItemView *view)
{
    return reinterpret_cast<QSortFilterProxyModel*>(view->model());
}

inline QSqlTableModel* viewSourceModel(QAbstractItemView *view)
{
    return reinterpret_cast<QSqlTableModel*>(viewProxyModel(view)->sourceModel());
}

inline QSqlRecord getSqlRecord(const QModelIndex &proxyIndex, QAbstractItemView *view)
{
    auto sourceIndex = viewProxyModel(view)->mapToSource(proxyIndex);
    return viewSourceModel(view)->record(sourceIndex.row());
}

inline QSqlRecord getSqlRecord(QAbstractItemView *view)
{
    return getSqlRecord(viewProxyModel(view)->index(0, 0, view->rootIndex()), view);
}
#endif // FETCHMODELINFO_H
