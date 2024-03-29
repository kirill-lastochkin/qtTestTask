#ifndef VALIDATIONTOOLS_H
#define VALIDATIONTOOLS_H

#include <QSqlTableModel>

bool isLiteral(const QString &str);
bool checkLengthExceeded(int len, int maxLen);
bool checkKeyDuplication(const QSqlTableModel* model, const QString &key, const QString &keyName);
bool checkEmptyValue(const QString &newValue);

#endif // VALIDATIONTOOLS_H
