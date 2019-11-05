#ifndef PROJECTSTAB_H
#define PROJECTSTAB_H

#include "commontab.h"

class ProjectsTab : public CommonTab
{
    Q_OBJECT

public:
    explicit ProjectsTab(QWidget *parent = nullptr);

    void setTableModel(QSqlTableModel *model) override;

public slots:
    void showProjectInfo(void);
    void setProjectFilterByCustomer(const QString &newText);

signals:
    void showProjectPressed(const QString &);
};

#endif // PROJECTSTAB_H
