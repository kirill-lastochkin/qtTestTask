#ifndef PROJECTINFOVIEW_H
#define PROJECTINFOVIEW_H

#include <QAbstractItemView>
#include <QSortFilterProxyModel>

#include "databasemaintainer.h"
#include "fetchmodelinfo.h"
#include "clickablelabel.h"

class ProjectInfoView : public QAbstractItemView
{
    Q_OBJECT

public:
    explicit ProjectInfoView(QWidget *parent = nullptr);

    QRect visualRect(const QModelIndex &index) const override;
    QModelIndex indexAt(const QPoint &point) const override;

signals:
    void keyValueChanged(const QString &newKey);

public slots:
    void updateText(void);

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;
    void startEditing(ClickableLabel *label);

protected:
    inline QSortFilterProxyModel* proxyModel(void) { return viewProxyModel(this); }

private:
    ClickableLabel* data[DatabaseMaintainer::projectsTableColumnCount];

    /* Necessary implementations of pure virtual functions */
public:
    void scrollTo(const QModelIndex&, ScrollHint = EnsureVisible) override {}

protected:
    QModelIndex moveCursor(CursorAction, Qt::KeyboardModifiers) override { return currentIndex(); }
    int horizontalOffset() const override { return 0; }
    int verticalOffset() const override { return 0; }
    bool isIndexHidden(const QModelIndex &) const override { return false; }
    void setSelection(const QRect &, QItemSelectionModel::SelectionFlags) override {}
    QRegion visualRegionForSelection(const QItemSelection &) const override { return QRegion(); }
};

#endif // PROJECTINFOVIEW_H
