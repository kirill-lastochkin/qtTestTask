#ifndef CUSTOMERSTAB_H
#define CUSTOMERSTAB_H

#include "commontab.h"

class CustomersTab : public CommonTab
{
    Q_OBJECT

public:
    explicit CustomersTab(QWidget *parent = nullptr);

private:
    void selectRowByKey(const QString &customer) override;
    void deselectRow(void) override;
};

#endif // CUSTOMERSTAB_H
