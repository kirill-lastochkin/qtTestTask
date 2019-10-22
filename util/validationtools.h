#ifndef VALIDATIONTOOLS_H
#define VALIDATIONTOOLS_H

#include <QSqlTableModel>
#include <QSqlRecord>

bool isLiteral(const QString &str);
bool checkLengthExceeded(const QString &newValue, int len, int maxLen);
bool checkKeyDuplication(const QSqlTableModel* model, const QString &key, const QString &keyName);
bool checkEmptyValue(const QString &newValue);

#endif // VALIDATIONTOOLS_H
