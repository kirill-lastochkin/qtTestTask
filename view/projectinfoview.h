#ifndef PROJECTINFOVIEW_H
#define PROJECTINFOVIEW_H

#include <QAbstractItemView>
#include <QLineEdit>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "databasemaintainer.h"
#include "fetchmodelinfo.h"

class ClickableLabel;

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

class ClickableLabel : public QLineEdit
{
    Q_OBJECT

public:
    explicit ClickableLabel(int col, QWidget *parent=nullptr)
        : QLineEdit(parent), tableColumn(col) { setReadOnly(true); }

    int column(void) const { return tableColumn; }

signals:
    void doubleClicked(ClickableLabel *self);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        QLineEdit::mouseDoubleClickEvent(event);
        emit doubleClicked(this);
    }

protected:
    int tableColumn;
};

#endif // PROJECTINFOVIEW_H
