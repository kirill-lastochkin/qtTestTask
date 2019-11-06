#include "projectinfoview.h"
#include "projectinfodelegate.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QMouseEvent>

ProjectInfoView::ProjectInfoView(QWidget *parent)
    : QAbstractItemView (parent)
{
    auto mainLayout = new QVBoxLayout;

    const char *headers[ProjectsColumns::projectsTableColumnCount] = {"Project name", "Customer", "Description", "Start date", "End date"};
    for (int column = 0; column < ProjectsColumns::projectsTableColumnCount; column++)
    {
        auto subLayout = new QVBoxLayout;
        auto label = new QLabel(tr(headers[column]));
        label->setAlignment(Qt::AlignTop);
        data[column] = new ClickableLabel(column);

        subLayout->addWidget(label);
        subLayout->addWidget(data[column]);
        mainLayout->addLayout(subLayout);

        connect(data[column], SIGNAL(doubleClicked(ClickableLabel *)), this, SLOT(startEditing(ClickableLabel *)));
    }

    setLayout(mainLayout);

    int minWidth = data[0]->width() / 2;
    int minHeight = ProjectsColumns::projectsTableColumnCount * 2 * 25;
    setMinimumSize(minWidth, minHeight);

    auto delegate = new ProjectInfoDelegate;
    setItemDelegate(delegate);
    connect(delegate, SIGNAL(editingFinished(void)), this, SLOT(updateText(void)));

    setModel(new QSortFilterProxyModel);
}

void ProjectInfoView::updateText(void)
{
    ProjectInfo info(getSqlRecord(this));

    for (int column = 0; column < ProjectsColumns::projectsTableColumnCount; column++)
    {
        if (!data[column]->isVisible())
            data[column]->setVisible(true);

        auto newValue = info[column].toString();
        auto oldValue = data[column]->text();

        if (newValue == oldValue)
            continue;

        data[column]->setText(newValue);

        if (column == ProjectsColumns::projectsTableKey)
            emit keyValueChanged(newValue);
    }
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

    for (int column = 0; column < ProjectsColumns::projectsTableColumnCount; column++)
    {
        const auto label = data[column];
        const auto &pos = label->pos();

        if (x >= pos.x() && x < pos.x() + label->width() - 1 &&
            y >= pos.y() && y < pos.y() + label->height() - 1)
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
