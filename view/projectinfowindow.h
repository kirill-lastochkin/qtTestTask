#ifndef PROJECTINFOWINDOW_H
#define PROJECTINFOWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QDialog>

#include "databasemaintainer.h"

class ProjectInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectInfoWindow(const DatabaseMaintainer::ProjectInfo &info, QWidget *parent = nullptr);
    ~ProjectInfoWindow();

signals:
    void closed(ProjectInfoWindow *window);

protected:
    void keyPressEvent(QKeyEvent *) override;

private:
    //QTextEdit *text;

    void closeEvent(QCloseEvent* event);
};

#endif // PROJECTINFOWINDOW_H
