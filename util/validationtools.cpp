#include "validationtools.h"

#include <QLoggingCategory>
#include <QString>
#include <QRegExp>

bool isLiteral(const QString &str)
{
    QRegExp reg("[^a-z A-Z]");
    return (reg.indexIn(str) == -1);
}

bool checkLengthExceeded(const QString &newValue, int len, int maxLen)
{
    if (len > maxLen)
    {
        qDebug() << "Length limit in string" << newValue << "is broken:" << len << ">" << maxLen;
        return true;
    }

    return false;
}

bool checkKeyDuplication(const QSqlTableModel* model, const QString &key, const QString &keyName)
{
    bool found = false;
    for (int row = 0; row < model->rowCount(); row++)
    {
        auto record = model->record(row);
        if (record.value(keyName) == key)
        {
            if (found)
            {
                qDebug() << "Name duplication found" << key << "in row" << row;
                return true;
            }
            else
            {
                found = true;
            }
        }
    }

    return false;
}

bool checkEmptyValue(const QString &newValue)
{
    if (newValue.isEmpty())
    {
        qDebug() << "Trying to set empty value";
        return true;
    }

    return false;
}
