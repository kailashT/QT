#include "create_bill.h"

create_bill::create_bill()
{
    qDebug() << __LINE__<< "Create bill object created";
}

create_bill::~create_bill()
{
    qDebug() << __LINE__<< "Create bill object Deleted";
}


int create_bill::bill_generate_for_single_customer(QStringList cust_data,QString fileName)
{
    qDebug() << __LINE__<< "Enter in bill_generate_for_single_customer";
    qDebug() << __LINE__<< "data in generate bill" << cust_data;


    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.newPage();
    QPainter painter(&pdfWriter);

    int space_ratio = 5;
    int font_ratio = 5;
    painter.setFont(QFont("Arial",(font_ratio * 4)));


    while(cust_data.size())
    {
        painter.drawText( space_ratio*680, space_ratio*220, "Shital Industries");
        painter.setFont(QFont("Arial", 16));
        QString bill_date  = "Date : ";
        bill_date.append(cust_data[9]);
        painter.drawText( space_ratio*1300, space_ratio*150, bill_date);
        painter.drawText( space_ratio*100, space_ratio*150, "Ph.:9850329755");
        painter.drawText( space_ratio*100, space_ratio*300, "Name : ");
        painter.drawText( space_ratio*300, space_ratio*300, cust_data[0]);
        painter.drawText( space_ratio*100, space_ratio*400, "Rate For Bottle : ");
        painter.drawText( space_ratio*500, space_ratio*400, cust_data[10]);
        painter.drawText( space_ratio*100, space_ratio*425, "_______________________________________________________");

        painter.drawText( space_ratio*100, space_ratio*500, cust_data[11]);
        QString str_common = cust_data[2];
        str_common.append(" Bottles");
        painter.drawText( space_ratio*1300, space_ratio*500, str_common);
        painter.drawText( space_ratio*100, space_ratio*600, "This Month Bill ");

        str_common.clear();
        str_common = cust_data[5];
        str_common.append(" Rs");

        painter.drawText( space_ratio*1300, space_ratio*600, str_common);

        painter.drawText( space_ratio*100, space_ratio*700, "Last Month Bill ");
        str_common.clear();
        str_common.append(cust_data[7]);
        str_common.append(" Rs");
        painter.drawText( space_ratio*1300, space_ratio*700, str_common);


        painter.drawText( space_ratio*100, space_ratio*800, "This Month Paid ");
        str_common.clear();
        str_common.append(cust_data[4]);
        str_common.append(" Rs");
        painter.drawText( space_ratio*1300, space_ratio*800, str_common);


        painter.drawText( space_ratio*100, space_ratio*900, "Last To pay");
        str_common.clear();
        str_common.append(cust_data[8]);
        str_common.append(" Rs");
        painter.drawText( space_ratio*1300, space_ratio*900, str_common);
        painter.drawText( space_ratio*100, space_ratio*925, "_______________________________________________________");

        painter.drawText( space_ratio*100, space_ratio*1000, "Total To pay");
        str_common.clear();
        str_common.append(cust_data[6]);
        str_common.append(" Rs");
        painter.drawText( space_ratio*1300, space_ratio*1000, str_common);

        painter.drawText( space_ratio*1300, space_ratio*1200, "Signature");

        for( int m=1; m <=12; m++)
        {
            cust_data.removeFirst();
        }

        pdfWriter.newPage();
    }
    qDebug() << __LINE__<< "Exit from bill_generate_for_single_customer";

    return 0;

}
