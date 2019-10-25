#ifndef PROJECTINFOVIEW_H
#define PROJECTINFOVIEW_H

#include <QAbstractItemView>
#include <QLineEdit>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "databasemaintainer.h"
#include "projectinfowindow.h"

class ClickableLabel;

class ProjectInfoView : public QAbstractItemView
{
    Q_OBJECT

    friend class ProjectInfoWindow;

public:
    explicit ProjectInfoView(QAbstractItemModel *model, const QString &key, QWidget *parent = nullptr);

    QRect visualRect(const QModelIndex &index) const override;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
    QModelIndex indexAt(const QPoint &point) const override;

protected:
    QModelIndex moveCursor(CursorAction cursorAction,
                                   Qt::KeyboardModifiers modifiers) override;

    int horizontalOffset() const override;
    int verticalOffset() const override;

    bool isIndexHidden(const QModelIndex &) const override;

    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;

    inline QSortFilterProxyModel* proxyModel(void)
    {
        return reinterpret_cast<QSortFilterProxyModel*>(model());
    }

    inline QSqlTableModel* sourceModel(void)
    {
        return reinterpret_cast<QSqlTableModel*>(proxyModel()->sourceModel());
    }

    //bool edit(const QModelIndex &index, QAbstractItemView::EditTrigger trigger, QEvent *event) override;
    //void mouseDoubleClickEvent(QMouseEvent *event) override;

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;
    void startEditing(ClickableLabel *label);
    void updateText(void);

private:
    //QLineEdit* data[DatabaseMaintainer::projectsTableColumnCount];
    ClickableLabel* data[DatabaseMaintainer::projectsTableColumnCount];
};

class ClickableLabel : public QLineEdit
{
    Q_OBJECT

public:
    explicit ClickableLabel(int col, QWidget *parent=nullptr)
        : QLineEdit(parent), tableColumn(col) { setReadOnly(true); }
    explicit ClickableLabel(const QString &text, int col, QWidget *parent=nullptr)
        : QLineEdit(text, parent), tableColumn(col) { setReadOnly(true); }

    int column(void) const { return tableColumn; }

signals:
    void doubleClicked(ClickableLabel *self);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

protected:
    int tableColumn;
};

#endif // PROJECTINFOVIEW_H
