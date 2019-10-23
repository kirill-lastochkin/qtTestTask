#ifndef PROJECTINFOWINDOW_H
#define PROJECTINFOWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>

#include "databasemaintainer.h"

class ProjectInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectInfoWindow(const QSqlTableModel *sourceModel, int row, QWidget *parent = nullptr);

    const QString getProject(void);
    const ProjectInfo getUpdatedInfo(void);

signals:
    void closed(ProjectInfoWindow *window);
    void accepted(ProjectInfoWindow *window);

private:
    ProjectInfo projectInfo;
    QLineEdit* data[DatabaseMaintainer::projectsTableColumnCount];
    QString oldProjectName;

    const QSqlTableModel *model;

    void closeEvent(QCloseEvent* event) override;
    void reject() override;
    void accept() override;
};

#endif // PROJECTINFOWINDOW_H
