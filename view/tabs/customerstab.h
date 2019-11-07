#ifndef CUSTOMERSTAB_H
#define CUSTOMERSTAB_H

#include "commontab.h"

class CustomersTab : public CommonTab
{
    Q_OBJECT

public:
    explicit CustomersTab(QWidget *parent = nullptr);

public slots:
    void addRow(int newRow) override;
};

#endif // CUSTOMERSTAB_H
