#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLineEdit>

class ClickableLabel : public QLineEdit
{
    Q_OBJECT

public:
    explicit ClickableLabel(int col, QWidget *parent=nullptr)
        : QLineEdit(parent), tableColumn(col) { setReadOnly(true); }

    int column(void) const { return tableColumn; }

signals:
    void doubleClicked(ClickableLabel *self);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        QLineEdit::mouseDoubleClickEvent(event);
        emit doubleClicked(this);
    }

protected:
    int tableColumn;
};

#endif // CLICKABLELABEL_H
