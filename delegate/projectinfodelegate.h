#ifndef PROJECTINFODELEGATE_H
#define PROJECTINFODELEGATE_H

#include "projectsdelegate.h"

#include <QWidget>
#include <QLoggingCategory>

class ProjectInfoDelegate : public ProjectsDelegate
{
    Q_OBJECT

public:
    explicit ProjectInfoDelegate(QWidget *parent = nullptr);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

signals:
    void editingFinished(void);

public slots:
    void finishEditing(void);
};

#endif // PROJECTINFODELEGATE_H
