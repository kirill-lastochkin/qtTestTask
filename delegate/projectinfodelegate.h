#ifndef PROJECTINFODELEGATE_H
#define PROJECTINFODELEGATE_H

#include "projectsdelegate.h"

#include <QWidget>
#include <QLoggingCategory>
#include <QLineEdit>

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
};

#endif // PROJECTINFODELEGATE_H
