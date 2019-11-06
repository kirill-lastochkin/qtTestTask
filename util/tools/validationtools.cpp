#include "validationtools.h"
#include "postmessage.h"

#include <QString>
#include <QRegExp>
#include <QSqlRecord>

bool isLiteral(const QString &str)
{
    QRegExp reg("[^a-z A-Z]");
    return (reg.indexIn(str) == -1);
}

bool checkLengthExceeded(int len, int maxLen)
{
    if (len > maxLen)
    {
        showMessage(QString("Maximum length is %1").arg(maxLen));
        return true;
    }

    return false;
}

bool checkKeyDuplication(const QSqlTableModel* model, const QString &key, const QString &keyName)
{
    for (int row = 0; row < model->rowCount(); row++)
    {
        auto record = model->record(row);
        if (record.value(keyName) == key)
        {
            showMessage(QString("Key '%1' matches with existing entry key in row %2").arg(key).arg(row + 1));
            return true;
        }
    }

    return false;
}

bool checkEmptyValue(const QString &newValue)
{
    if (newValue.isEmpty())
    {
        showMessage("Trying to set empty value");
        return true;
    }

    return false;
}
