#include "postmessage.h"

#include <QMessageBox>
#include <QLoggingCategory>

static class PostMessage
{
public:
    PostMessage() = default;
    void setOwner(QWidget *parent) { owner = parent; }
    void showMessage(const QString &message)
    {
        QMessageBox msg(owner);
        msg.setWindowTitle("Warning");
        msg.setText(message);
        msg.exec();

        qWarning() << message;
    }

private:
    QWidget *owner = nullptr;
} messenger;

void showMessage(const QString &message)
{
    messenger.showMessage(message);
}

void registerMessagesParentWindow(QWidget *parent)
{
    messenger.setOwner(parent);
}
