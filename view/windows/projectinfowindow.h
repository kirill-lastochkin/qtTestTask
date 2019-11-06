#ifndef PROJECTINFOWINDOW_H
#define PROJECTINFOWINDOW_H

#include <QDialog>
#include <QSortFilterProxyModel>

#include "projectinfoview.h"
#include "fetchmodelinfo.h"

class ProjectInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectInfoWindow(QWidget *parent = nullptr);

    void setModel(QSqlTableModel *model, const QString &key);
    void saveCurrentProjectInfo(void);

    const ProjectInfo& savedProjectInfo(void) { return oldProjectInfo; }
    const QString& key(void) { return newKey; }

signals:
    void canceled(ProjectInfoWindow *window);
    void accepted(ProjectInfoWindow *window);

private slots:
    void changeKeyValue(const QString &key);

private:
    ProjectInfo oldProjectInfo;
    ProjectInfoView *view;
    QString newKey;

    void closeEvent(QCloseEvent* event) override;
    void reject() override;
    void accept() override;

    inline QSortFilterProxyModel* proxyModel(void) { return viewProxyModel(view); }

    static const int windowWidthDefault;
    static const int windowHeightDefault;
};

#endif // PROJECTINFOWINDOW_H
