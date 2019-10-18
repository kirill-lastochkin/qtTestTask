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

    friend class ProjectsTab;

public:
    explicit ProjectInfoWindow(const DatabaseMaintainer::ProjectInfo &info, QWidget *parent = nullptr);

    const QString getProject(void);
    const QString getCustomer(void);

signals:
    void closed(ProjectInfoWindow *window);
    void accepted(ProjectInfoWindow *window);

private:
    QLineEdit *editProject;
    QLineEdit *editCustomer;
    QLineEdit *editStart;
    QLineEdit *editEnd;
    QLineEdit *editDesc;

    void closeEvent(QCloseEvent* event) override;
    void reject() override;
    void accept() override;
};

#endif // PROJECTINFOWINDOW_H
