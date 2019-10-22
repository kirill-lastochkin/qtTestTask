#ifndef PROJECTINFOWINDOW_H
#define PROJECTINFOWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QDialog>
#include <QLineEdit>

#include "databasemaintainer.h"

class ProjectInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectInfoWindow(const ProjectInfo &info, QWidget *parent = nullptr);

    const QString getProject(void);
    const ProjectInfo getUpdatedInfo(void);

signals:
    void closed(ProjectInfoWindow *window);
    void accepted(ProjectInfoWindow *window);

private:
    QLineEdit *editProject;
    QLineEdit *editCustomer;
    QLineEdit *editStart;
    QLineEdit *editEnd;
    QLineEdit *editDesc;

    QString oldProjectName;

    void closeEvent(QCloseEvent* event) override;
    void reject() override;
    void accept() override;
};

#endif // PROJECTINFOWINDOW_H
