#ifndef POSTMESSAGE_H
#define POSTMESSAGE_H

#include <QString>
#include <QWidget>

void registerMessagesParentWindow(QWidget *parent);
void showMessage(const QString &message);

#endif // POSTMESSAGE_H
