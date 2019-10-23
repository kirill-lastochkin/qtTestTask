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
    void hideProjectInfo(ProjectInfoWindow *window);
    void saveProjectInfo(ProjectInfoWindow *window);

    void setProjectFilterByCustomer(const QString &newText);

private:
    QLineEdit *editCustomerFilter;
    const int maxProjectInfoWinNumber = 4;
    static int openedWinCount;

    void selectRowByKey(const QString &project) override;
    void deselectRow(void) override;
};

#endif // PROJECTSTAB_H
