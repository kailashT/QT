#ifndef CREATE_BILL_H
#define CREATE_BILL_H

#include <QPdfWriter>
#include <QDebug>
#include <QPainter>

class create_bill
{
public:
    create_bill();
    ~create_bill();
    int bill_generate_for_single_customer(QStringList,QString);

};

#endif // CREATE_BILL_H
