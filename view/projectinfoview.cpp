#include "projectinfoview.h"
#include "projectinfodelegate.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QMouseEvent>

ProjectInfoView::ProjectInfoView(QAbstractItemModel *model, const QString &key, QWidget *parent)
    : QAbstractItemView (parent)
{
    //UI
    auto mainLayout = new QVBoxLayout;

    const char *headers[DatabaseMaintainer::projectsTableColumnCount] = {"Project name", "Customer", "Description", "Start date", "End date"};
    int elementCount = 0;
    for (int column = 0; column < DatabaseMaintainer::projectsTableColumnCount; column++)
    {
        auto subLayout = new QVBoxLayout;
        auto label = new QLabel(tr(headers[column]));
        label->setAlignment(Qt::AlignTop);
        data[column] = new ClickableLabel(column);

        subLayout->addWidget(label);
        subLayout->addWidget(data[column]);
        mainLayout->addLayout(subLayout);

        connect(data[column], SIGNAL(doubleClicked(ClickableLabel *)), this, SLOT(startEditing(ClickableLabel *)));

        elementCount += 2;
    }

    setLayout(mainLayout);

    int elementHeight = 25;
    setMinimumSize(data[0]->width() / 2, elementCount * elementHeight);

    //setup model

    setModel(new QSortFilterProxyModel);
    proxyModel()->setSourceModel(model);
    proxyModel()->setFilterKeyColumn(DatabaseMaintainer::ProjectsTableColumn::project);
    proxyModel()->setFilterRegExp(key + "$");
    updateText();

    //delegate

    auto delegate = new ProjectInfoDelegate;
    setItemDelegate(delegate);
    connect(delegate, SIGNAL(editingFinished(void)), this, SLOT(updateText(void)));
}

void ProjectInfoView::startEditing(ClickableLabel *label)
{
    auto index = proxyModel()->index(0, label->column(), rootIndex());
    if (index.isValid())
    {
        label->hide();
        edit(index);
    }
}

void ClickableLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    QLineEdit::mouseDoubleClickEvent(event);
    emit doubleClicked(this);
}

void ProjectInfoView::updateText(void)
{
    int row = 0, column = 0;
    auto proxyIndex = proxyModel()->index(row, column, rootIndex());
    auto sourceIndex = proxyModel()->mapToSource(proxyIndex);

    ProjectInfo info(sourceModel()->record(sourceIndex.row()));

    for (column = 0; column < DatabaseMaintainer::projectsTableColumnCount; column++)
    {
        data[column]->setText(info[column].toString());
        if (!data[column]->isVisible())
            data[column]->setVisible(true);
    }
}

QRect ProjectInfoView::visualRect(const QModelIndex &index) const
{
    if (!index.isValid())
        return QRect();

    const auto label = data[index.column()];
    const auto &p = label->pos();

    return QRect(p.x(), p.y(), label->width(), label->height());
}

QModelIndex ProjectInfoView::indexAt(const QPoint &point) const
{
    int x = point.x();
    int y = point.y();

    for (int column = 0; column < DatabaseMaintainer::projectsTableColumnCount; column++)
    {
        const auto label = data[column];
        const auto &p = label->pos();

        if (x >= p.x() && x < p.x() + label->width() - 1 &&
            y >= p.y() && y < p.y() + label->height() - 1)
        {
            int row = 0;
            return model()->index(row, column, rootIndex());
        }
    }

    return QModelIndex();
}

void ProjectInfoView::dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)
{
    updateText();
}

QModelIndex ProjectInfoView::moveCursor(CursorAction, Qt::KeyboardModifiers)
{
    return currentIndex();
}

int ProjectInfoView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int ProjectInfoView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool ProjectInfoView::isIndexHidden(const QModelIndex &) const
{
    return false;
}

void ProjectInfoView::scrollTo(const QModelIndex &, ScrollHint)
{
    return;
}

void ProjectInfoView::setSelection(const QRect &, QItemSelectionModel::SelectionFlags)
{
    return;
}

QRegion ProjectInfoView::visualRegionForSelection(const QItemSelection &) const
{
    return QRegion();
}
