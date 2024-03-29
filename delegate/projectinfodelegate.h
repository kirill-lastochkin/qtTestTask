#ifndef PROJECTINFODELEGATE_H
#define PROJECTINFODELEGATE_H

#include "projectsdelegate.h"

class ProjectInfoDelegate : public ProjectsDelegate
{
    Q_OBJECT

public:
    explicit ProjectInfoDelegate(QWidget *parent = nullptr)
        : ProjectsDelegate(parent) {}

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

signals:
    void editingFinished(void);
};

#endif // PROJECTINFODELEGATE_H
