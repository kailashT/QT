    #include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * DB me muze jo chahiye wo hai ki nai
 * check for tables
 * Customer_Info_Table table

*/



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Timer init date time
    digital_timer = new QTimer(this);
    QObject::connect(digital_timer,SIGNAL(timeout()),this,SLOT(UpdateTime()));
    digital_timer->start(1000);

    //Sqlite database init
    ui->setupUi(this);
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        QMessageBox::critical(this, "Database Error", "This demo needs the SQLITE drivers");
        exit(0);
    }

    db_edit = new sqlitedb();
    check_for_database();


    wordListname = db_edit->Get_names();
    QCompleter *completer = new QCompleter(wordListname, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->Value_name->setCompleter(completer);
    ui->User_name->setCompleter(completer);
    ui->Search_name->setCompleter(completer);
    ui->Name_for_bill->setCompleter(completer);

    QStringList wordListmobile ;
    wordListmobile = db_edit->Get_Mobile();
    QCompleter *mobile_completer = new QCompleter(wordListmobile, this);
    mobile_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->User_mo1->setCompleter(mobile_completer);


    //ui init
    ui->Date->setText(QDate::currentDate().toString());   //ui->label_time->setText(QTime::currentTime().toString("hh:mm"));
    ui->Time->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->Set_date->setDate((QDate::currentDate()));
    ui->Date_Show->setDate((QDate::currentDate()));

    Bill = new create_bill();


}


void MainWindow::check_for_database()
{
    //check each table if exist or not
    int ret = db_edit->If_Table_Exists("Customer_Info_Table");
    if(ret)
    {
        QSqlError err = db_edit->Execute_query(CUSTOMERTABLE);
        if (err.type() != QSqlError::NoError)
        {
            qCritical() << __LINE__ <<  "Create CUSTOMERTABLE fails" << err;
            QMessageBox::critical(this,"DataBase Error", "database problem","close");
            this->close();
        }
    }
    ret = db_edit->If_Table_Exists("Daily_Data_Entry_Table");
    if(ret)
    {
        QSqlError err = db_edit->Execute_query(DAILY_DATA_ENTRY_TABLE);
        if (err.type() != QSqlError::NoError)
        {
            qCritical() << __LINE__ <<  "Create DAILY_DATA_ENTRY_TABLE fails" << err;
            QMessageBox::critical(this,"DataBase Error", "database problem","close");
            this->close();
        }
    }
    ret = db_edit->If_Table_Exists("Monthly_Data_Tracking_Table");
    if(ret)
    {
        QSqlError err = db_edit->Execute_query(MONTHLY_DATA_TRACKING_TABLE);
        if (err.type() != QSqlError::NoError)
        {
            qCritical() << __LINE__ <<  "Create MONTHLY_DATA_TRACKING_TABLE fails" << err;
            QMessageBox::critical(this,"DataBase Error", "database problem","close");
            this->close();
        }
    }
    qDebug() << __LINE__ << "tables : " <<  db_edit->Get_Table_Names();
}



MainWindow::~MainWindow()
{
    db_edit->~sqlitedb();
    delete ui;
    delete Bill;
}

//Daily data table edit
void MainWindow::on_Save_daily_data_clicked()
{
    QString insert_command = "";

    if(ui->SU_Record->currentData(0).toString() == "Save Record")
    {
        //SELECT EXISTS(SELECT 1 FROM Daily_data_entry_table WHERE id=(select id from Customer_info_table where name='rakesh') and date='18-12-2018' );
        //SELECT EXISTS(SELECT 1 FROM Daily_data_Entry_Table  WHERE id='15' and date='18-12-2018' ); asel tr 1 nasel tr 0

        bool *ok = NULL;
        insert_command.append("SELECT EXISTS(SELECT 1 FROM Daily_data_entry_table WHERE name='");
        insert_command.append(ui->Value_name->text());
        insert_command.append("' and date='");
        insert_command.append( (QDateTime::fromString(ui->Set_date->text(), "dd-MM-yyyy")).toString("yyyy-MM-dd"));
        insert_command.append("' );");
        qDebug() << __LINE__ << "insert_command : " << insert_command;

        QStringList return_list = db_edit->get_return_value(insert_command);
        qCritical() << __LINE__ <<  "data exists status : " << return_list[0];
        insert_command.clear();

        // TODO ask before Direct Data update insted of Inserting
        if(return_list[0].toInt(ok,10))
        {
            qDebug() << __LINE__ << "update" ;
            insert_command.append(Update_Query_For_Daily_data_Table());
            qDebug() << __LINE__ << "Update_command :" << insert_command ;
        }else
        {
            qDebug() << __LINE__ << "insert";
            insert_command.append(Insart_Query_For_Daily_data_Table());
            qDebug() << __LINE__<< "insert_command :" << insert_command ;
        }

    }
    else if(ui->SU_Record->currentData(0).toString() == "Update Record")
    {
        qDebug() << __LINE__<< "update" ;
        insert_command.append(Update_Query_For_Daily_data_Table());
        qDebug() << __LINE__<< "insert_command :" << insert_command ;
    }

    QSqlError err = db_edit->Execute_query(insert_command);
    if (err.type() != QSqlError::NoError)
    {
        qCritical() << __LINE__ <<  "Daily data update fail" << err;
        QMessageBox::critical(this,"Error", "Daily data update fail","ok");

    }else
    {
        //QMessageBox::critical(this,"Info", "Data saved","ok");
        QMessageBox::information(this,"Info", "Data Updated Successfully","ok");
    }

    qDebug() << __LINE__<< "Today date: " << ui->Set_date->text();
    qDebug() << __LINE__<< "Value_name: " << ui->Value_name->text();
    qDebug() << __LINE__<< "value_taken: " << ui->value_taken->text();
    qDebug() << __LINE__<< "value_return: " << ui->value_return->text();
    qDebug() << __LINE__<< "value_money: " << ui->value_money->text();

    ui->Value_name->clear();
    //ui->value_taken->clear();
    //ui->value_return->clear();
    ui->value_money->clear();

    ui->stackedWidget->setCurrentWidget(ui->Daily_Stat);


}

void MainWindow::on_btn_add_user_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Add_User);
    ui->User_name->clear();
    ui->User_address->clear();
    ui->User_mo1->clear();
    ui->User_mo2->clear();
    ui->User_price->clear();
}

void MainWindow::on_btn_daily_data_clicked()
{
    ui->Value_name->clear();
    ui->value_money->clear();
    ui->value_taken->setText("1");
    ui->value_return->setText("1");
    ui->stackedWidget->setCurrentWidget(ui->Daily_Stat);
}

void MainWindow::on_btn_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Welcome);
}

//customer data table edit
/*
 * ADD:
 *      input:
 *              Name(UNIQUE) , Mobile no, Price
 *      procedure:
 *              check Name mobile no and price are provided
 *              execute query:
 *                  "INSERT INTO Customer_Info_Table (Name, Address,MoNumber,MoNumber2,Price) VALUES ('rajesh','Smarti systems','9822583254','9876549315','50');"
 *              To add lst month balance
 *              execute query
 *                  insert into Customer_Info_Table (id,name,month,To_Be_Paid_In_This_Month) values ((select id from Customer_info_table where name="rakesh"), 'rakesh', '12-2018', '1500');
 *      output:
 *              Customer Data  added
 *              last month balance added to monthly data tracking table
 * Delete:
 *      input:
 *      Procedure:
 *      output:
 *
 * Update:
 *      input:
 *          Name(UNIQUE) , Mobile no, Price
 *      Procedure:
 *          update is done on the basis of mobile no
 *          check Name mobile no and price are provided
 *          execute Query
 *              "UPDATE Customer_Info_Table SET Name = 'ui->User_name->text()', Address = 'ui->User_address->text()', MoNumber2 = 'ui->User_mo2->text()', Price = 'ui->User_price->text()' WHERE MoNumber = 'ui->User_mo1->text()'
 *      output:
*/
void MainWindow::on_Save_Data_clicked()
{
    qDebug() << __LINE__ <<  ui->Select_Action->currentData(0);
    QString Action = ui->Select_Action->currentData(0).toString();
    QString insert_command = "";
    bool *ok = NULL;
    if(Action == "Add")
    {
        //Mobile no name and price are  necessary
        if ((ui->User_name->text() == "") | (ui->User_price->text() == "") | (ui->User_mo1->text() == ""))
        {
            //show message boox * credentials are necessary
            QMessageBox::critical(this, "Credential Error","* Fields are necessary","OK");
            return ;
        }

        insert_command = "INSERT INTO Customer_Info_Table (Name, Address,MoNumber,MoNumber2,Price) VALUES ('" ;   //rajesh','Smarti systems','9822583254','9876549315','50');"
        insert_command.append(ui->User_name->text().simplified());
        insert_command.append("','");
        insert_command.append(ui->User_address->text());
        insert_command.append("','");
        insert_command.append(ui->User_mo1->text());
        insert_command.append("','");
        insert_command.append(ui->User_mo2->text());
        insert_command.append("','");
        insert_command.append(ui->User_price->text());
        insert_command.append("')");
        qDebug() << __LINE__ << "insert_command : " << insert_command;

        QSqlError err = db_edit->Execute_query(insert_command);
        if (err.type() != QSqlError::NoError)
        {
            qCritical() << __LINE__ <<  "Insert customer fails text: " <<  err.text() << "number" << err.number() << err.type();
            if(err.text().contains("UNIQUE"))
                QMessageBox::critical(this,"Error", "Duplicate Values","close");
            delete ok;
            return ;
        }

        //insert into Monthly_Data_Tracking_Table (id,name,month,To_Be_Paid_In_This_Month) values ((select id from Customer_info_table where name="rakesh"), 'rakesh',
        //'12-2018', '1500', '0');
        insert_command = "insert into Monthly_Data_Tracking_Table (id,Name,month,To_Be_Paid_In_This_Month,Last_To_Pay) values ((select id from Customer_info_table where name='";
        insert_command.append(ui->User_name->text());
        insert_command.append("'), '");
        insert_command.append(ui->User_name->text());
        insert_command.append("', '");
        insert_command.append((QDate::currentDate().addMonths(-1)).toString("MM-yyyy"));
        insert_command.append("', '");

        if(ui->To_pay->text() == "")
        {
            qDebug() << __LINE__ <<  "To pay value will be set to 0";
            ui->To_pay->setText("0");
        }else
        {

            qDebug() <<  "ui->To_pay->text() : " << ui->To_pay->text();
        }
        insert_command.append(ui->To_pay->text());
        insert_command.append("', '");
        insert_command.append(ui->To_pay->text());
        insert_command.append("');");
        qDebug() << "insert_command : " << insert_command;

        err = db_edit->Execute_query(insert_command);
        if (err.type() != QSqlError::NoError)
        {
            qCritical() << __LINE__ <<  "Old balance add failed : " <<  err.text() << "number" << err.number() << err.type();
            QMessageBox::critical(this,"DataBase Error", "Old balance add failed","close");
        }else
        {
            QMessageBox::information(this, "Success", "Customer Data Saved successfully","OK");
        }

        ui->User_name->clear();
        ui->User_address->clear();
        ui->User_mo1->clear();
        ui->User_mo2->clear();
        ui->User_price->clear();
        ui->To_pay->clear();

        ui->stackedWidget->setCurrentWidget(ui->Add_User);
        //qDebug() << "User_name: " << ui->User_name->text() << "User_address: " << ui->User_address->text() << "User_mo1: " << ui->User_mo1->text() << "User_mo2: " << ui->User_mo2->text() << "User_ref: " << ui->User_price->text();
    }
    else if(Action == "Delete")
    {
        //TODO Delete for Customer data to be added latter in worse case to be use
        QMessageBox::information(this, "Credential Error","* Fields are necessary","OK");
        qDebug() << __LINE__ <<  "Delete Action not Added";
    // update is done on the basis of mobile no
    }
    else if(Action == "Update")
    {
        if ((ui->User_name->text() == "") | (ui->User_price->text() == "") | (ui->User_mo1->text() == ""))
        {
            //show message boox * credentials are necessary
            QMessageBox::critical(this, "Credential Error","* Fields are necessary","OK");
            return ;
        }
        //SELECT EXISTS(SELECT 1 FROM Customer_Info_Table  WHERE MoNumber='9225922747' )
        insert_command.append("SELECT EXISTS(SELECT 1 FROM Customer_Info_Table  WHERE MoNumber='");
        insert_command.append(ui->User_mo1->text());
        insert_command.append("') ");
        qDebug() << __LINE__ << "insert_command : " << insert_command;

        QStringList return_value = db_edit->get_return_value(insert_command);
        qCritical() << __LINE__ <<  "data exists status : " << return_value[0];
        insert_command.clear();

        if(return_value[0].toInt(ok,10))
        {
            //"UPDATE names SET firstname = 'Nisse', lastname = 'Svensson' WHERE id = 7"
            insert_command = "UPDATE Customer_Info_Table SET Name = '";
            insert_command.append(ui->User_name->text());
            insert_command.append("', ");

            if( ui->User_address->text() != "")
            {
                insert_command.append("Address ='");
                insert_command.append(ui->User_address->text());
                insert_command.append("', ");
            }
            if( ui->User_mo2->text() != "")
            {
                insert_command.append("MoNumber2 = '");
                insert_command.append(ui->User_mo2->text());
                insert_command.append("', ");
            }
            insert_command.append("Price = '");
            insert_command.append(ui->User_price->text());
            insert_command.append("' WHERE MoNumber = '");
            insert_command.append(ui->User_mo1->text());
            insert_command.append("'");

            qDebug() << "insert_command : " << insert_command;

            QSqlError err = db_edit->Execute_query(insert_command);
            if (err.type() != QSqlError::NoError)
            {
                qCritical() << __LINE__ <<  "Update Data Failed : " <<  err.text() << "number" << err.number() << err.type();
                QMessageBox::critical(this,"DataBase Error", "Update Customer Data Failed","close");
            }else
            {
                QMessageBox::information(this, "Success", "Customer Data Updated successfully","OK");
                ui->User_name->clear();
                ui->User_address->clear();
                ui->User_mo1->clear();
                ui->User_mo2->clear();
                ui->User_price->clear();
            }
        }else {
            QMessageBox::information(this, "Success", "Customer Data Updated Failed check for mobile no","OK");
        }

        ui->stackedWidget->setCurrentWidget(ui->Add_User);
    }
    delete ok;
}


void MainWindow::UpdateTime()
{
    ui->Date->setText(QDate::currentDate().toString());
    ui->Time->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::on_btn_overview_clicked()
{
    ui->Date_Show->setVisible(true);
    ui->Search_name->setVisible(false);
    ui->label_11->setVisible(false);
    ui->stackedWidget->setCurrentWidget(ui->Overview);

}

void MainWindow::on_btn_add_user_pressed()
{
    ui->stackedWidget->setCurrentWidget(ui->Add_User);
}

void MainWindow::on_Destroy_session_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Welcome);
}












void MainWindow::on_Show_details_clicked()
{
    qDebug() << __LINE__ << "Search_name: " << ui->Search_name->text();
    qDebug() << __LINE__ << "Search_Date: " << ui->Date_Show->text();
    QString insert_command = "";
    int RowCount, ColumnCount, i;

    QString Action = ui->SAFSD->currentData(0).toString();
    qDebug() << __LINE__ << "Action : " << Action;
    QStringList wordList;
    QStringList labels;
    bool *ok = NULL;

    qDebug() << __LINE__ << "ui->Info_table->rowCount(): " << ui->Info_table->rowCount();
    qDebug() << __LINE__ << "ui->Info_table->columnCount(): " << ui->Info_table->columnCount();

    //Init Table
    Delete_Table_row();
    Delete_Table_Column();
    ui->Info_table->clearContents();
    ui->Info_table->clear();
    qDebug() << __LINE__ << "ui->Info_table->rowCount(): " << ui->Info_table->rowCount();
    qDebug() << __LINE__ << "ui->Info_table->columnCount(): " << ui->Info_table->columnCount();

    //Aquire Data
    if(Action == "Data For Single Date")
    {
        //select * from Daily_Data_Entry_Table where DATE='18-12-2018'

        insert_command.append("SELECT * from Daily_Data_Entry_Table where DATE='");
        insert_command.append((QDateTime::fromString(ui->Date_Show->text(), "dd-MM-yyyy")).toString("yyyy-MM-dd"));
        insert_command.append("'");
        qDebug() << __LINE__<< "insert_command :" << insert_command ;


        wordList = db_edit->Data_For_Single_Day(insert_command);
        qCritical() << __LINE__<<  "data exists status : " << wordList;

        labels << tr("Name") << tr("Date") << tr("Transfer") << tr("Received") << tr("Paid");
        qDebug() << __LINE__<< "labels :" << labels ;
        ColumnCount = labels.size();
        for(i = 0; i < ColumnCount; i++)
        {
            ui->Info_table->insertColumn(i);
        }
        ui->Info_table->setHorizontalHeaderLabels(labels);

        RowCount = wordList.size();
        qDebug() << __LINE__<< "circle :" << RowCount ;

        for( i = 0; i < (RowCount/5); i++)
        {
            qDebug() << __LINE__<< "i :" << i ;
            ui->Info_table->insertRow(i);

            ui->Info_table->setItem(i, 0, new QTableWidgetItem( wordList[ (5 * i) + 0] ));
            ui->Info_table->setItem(i, 1, new QTableWidgetItem( wordList[ (5 * i) + 1] ));
            ui->Info_table->setItem(i, 2, new QTableWidgetItem( wordList[ (5 * i) + 2] ));
            ui->Info_table->setItem(i, 3, new QTableWidgetItem( wordList[ (5 * i) + 3] ));
            ui->Info_table->setItem(i, 4, new QTableWidgetItem( wordList[ (5 * i) + 4] ));

        }
        ui->Info_table->setSortingEnabled(true);
        ui->Info_table->sortByColumn(0, Qt::AscendingOrder);
        ui->Info_table->setSortingEnabled(true);
        ui->Info_table->show();

    }
    else if(Action == "All Customer Information")
    {//SELECT * from Customer_Info_Table

        insert_command.append("SELECT * from Customer_Info_Table");
        qDebug() << __LINE__<< "insert_command :" << insert_command ;

        wordList = db_edit->get_all_customer_data(insert_command);
        qDebug() << __LINE__<< "All Customer Data :" << wordList ;

        //Table init
        labels << tr("Name") << tr("Address") << tr("Mobile1") << tr("Mobile2") << tr("Price");
        qDebug() << __LINE__<< "labels :" << labels ;
        ColumnCount = labels.size();
        for(i = 0; i < ColumnCount; i++)
        {
            ui->Info_table->insertColumn(i);
        }
        ui->Info_table->setHorizontalHeaderLabels(labels);

        RowCount = wordList.size();
        qDebug() << __LINE__<< "circle :" << RowCount ;

        for(int i =0; i < (RowCount/5); i++)
        {
            qDebug() << __LINE__<< "i :" << i ;
            ui->Info_table->insertRow(i);

            ui->Info_table->setItem(i, 0, new QTableWidgetItem( wordList[ (5 * i) + 0] ));
            ui->Info_table->setItem(i, 1, new QTableWidgetItem( wordList[ (5 * i) + 1] ));
            ui->Info_table->setItem(i, 2, new QTableWidgetItem( wordList[ (5 * i) + 2] ));
            ui->Info_table->setItem(i, 3, new QTableWidgetItem( wordList[ (5 * i) + 3] ));
            ui->Info_table->setItem(i, 4, new QTableWidgetItem( wordList[ (5 * i) + 4] ));
        }
        ui->Info_table->setSortingEnabled(true);
        ui->Info_table->sortByColumn(0, Qt::AscendingOrder);
        ui->Info_table->setSortingEnabled(true);

        ui->Info_table->show();
    }
    else if(Action == "Customer Monthly Data")
    {
        //TODO data between date is not working: wordListname
        //select * from Daily_Data_Entry_Table where name='rakesh' and date BETWEEN '1-11-2018' AND '31-11-2018'
        QString Month_no = (QDateTime::fromString(ui->Date_Show->text(), "dd-MM-yyyy")).toString("MM");
        QString Year_no = (QDateTime::fromString(ui->Date_Show->text(), "dd-MM-yyyy")).toString("yyyy");

        if(wordListname.contains(ui->Search_name->text()))
        {
            insert_command.append("select * from Daily_Data_Entry_Table where name='");
            insert_command.append(ui->Search_name->text());
            insert_command.append("' and date BETWEEN '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-");
            insert_command.append("01' AND '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-31'");

            qDebug() << __LINE__<< "insert_command :" << insert_command ;

            wordList = db_edit->get_all_customer_data(insert_command);
            qDebug() << __LINE__<< "All Customer Data :" << wordList ;


            //Table init
            labels << tr("Name") << tr("Date") << tr("Transfer") << tr("Received") << tr("Paid");
            qDebug() << __LINE__<< "labels :" << labels ;
            ColumnCount = labels.size();
            for(i = 0; i < ColumnCount; i++)
            {
                ui->Info_table->insertColumn(i);
            }
            ui->Info_table->setHorizontalHeaderLabels(labels);

            RowCount = wordList.size();
            qDebug() << __LINE__<< "circle :" << RowCount ;

            for(int i =0; i < (RowCount/5); i++)
            {
                qDebug() << __LINE__<< "i :" << i ;
                ui->Info_table->insertRow(i);

                ui->Info_table->setItem(i, 0, new QTableWidgetItem( wordList[ (5 * i) + 0] ));
                ui->Info_table->setItem(i, 1, new QTableWidgetItem( wordList[ (5 * i) + 1] ));
                ui->Info_table->setItem(i, 2, new QTableWidgetItem( wordList[ (5 * i) + 2] ));
                ui->Info_table->setItem(i, 3, new QTableWidgetItem( wordList[ (5 * i) + 3] ));
                ui->Info_table->setItem(i, 4, new QTableWidgetItem( wordList[ (5 * i) + 4] ));

            }
            //ui->Info_table->sortByColumn(0, Qt::AscendingOrder);
            //ui->Info_table->setSortingEnabled(true);

            ui->Info_table->show();
        }
        else
        {
            QMessageBox::critical(this,"Credentials Problem", "Please Give Proper Name","OK");
        }

    }else if(Action == "Sync For Customer")
    {
        if(wordListname.contains(ui->Search_name->text()))
        {
            Sync_For_A_Customer(ui->Search_name->text(),ui->Date_Show->text());
            QMessageBox::information(this,"Completed", "Sync Completed","OK");
        }else
        {
           QMessageBox::critical(this,"Credentials Problem", "Please Give Proper Name","OK");
        }

    }else if(Action == "Sync For All")
    {   //Sync For All
        //select name from Customer_info_table
        insert_command.append("select name from Customer_info_table");
        qDebug() << __LINE__<< "insert_command :" << insert_command ;
        QStringList return_list = db_edit->get_return_value(insert_command);
        qCritical() << __LINE__ <<  "Name of all customer: " << return_list;
        int list_count = return_list.count();
        for (int f=0; f < list_count; f++)
        {
            Sync_For_A_Customer(return_list[f],ui->Date_Show->text());
        }
        QMessageBox::information(this,"Completed", "Sync Completed","OK");

    }else if(Action == "Monthly report")
    {
        //select * from Monthly_Data_Tracking_Table where month='12-18'

        insert_command.append("SELECT * from Monthly_Data_Tracking_Table where MONTH='");
        insert_command.append((QDateTime::fromString(ui->Date_Show->text(), "dd-MM-yyyy")).toString("MM-yyyy"));
        insert_command.append("'");
        qDebug() << __LINE__<< "insert_command :" << insert_command ;
/**********************************************************************************/
        wordList = db_edit->Data_For_Monthly_Report(insert_command);
        qCritical() << __LINE__<<  "Data_For_Monthly_Report : " << wordList;

        labels << tr("Name") << tr("Month") << tr("Total Transfer") << tr("Total Return") << tr("Total Money Paid") << tr("Current Month Bill") << tr("To Pay") << tr("Last_Month_Bill") << tr("Last_To_Pay");
        qDebug() << __LINE__<< "labels :" << labels ;
        ColumnCount = labels.size();
        qDebug() << __LINE__<< "ColumnCount :" << ColumnCount ;
        for(i = 0; i < ColumnCount; i++)
        {
            ui->Info_table->insertColumn(i);
        }
        ui->Info_table->setHorizontalHeaderLabels(labels);

        RowCount = wordList.size();
        qDebug() << __LINE__<< "circle :" << RowCount ;

        //TODO change 9 to columncount by testing
        for(i = 0; i < (RowCount/9); i++)
        {
            qDebug() << __LINE__<< "i :" << i ;
            ui->Info_table->insertRow(i);

            ui->Info_table->setItem(i, 0, new QTableWidgetItem( wordList[ (9 * i) + 0] ));
            ui->Info_table->setItem(i, 1, new QTableWidgetItem( wordList[ (9 * i) + 1] ));
            ui->Info_table->setItem(i, 2, new QTableWidgetItem( wordList[ (9 * i) + 2] ));
            ui->Info_table->setItem(i, 3, new QTableWidgetItem( wordList[ (9 * i) + 3] ));
            ui->Info_table->setItem(i, 4, new QTableWidgetItem( wordList[ (9 * i) + 4] ));
            ui->Info_table->setItem(i, 5, new QTableWidgetItem( wordList[ (9 * i) + 5] ));
            ui->Info_table->setItem(i, 6, new QTableWidgetItem( wordList[ (9 * i) + 6] ));
            ui->Info_table->setItem(i, 7, new QTableWidgetItem( wordList[ (9 * i) + 7] ));
            ui->Info_table->setItem(i, 8, new QTableWidgetItem( wordList[ (9 * i) + 8] ));

        }
        //ui->Info_table->sortByColumn(0, Qt::AscendingOrder);
        //ui->Info_table->setSortingEnabled(true);
        ui->Info_table->show();
    }
    else if(Action == "Customer Monthly Report")
    {
        //TODO data between date is not working
        //TODO change 2018 make it general
        if(wordListname.contains(ui->Search_name->text()) )
        {
            qDebug() << __LINE__<< "ui->Search_name->text() :" << ui->Search_name->text() ;
            qDebug() << __LINE__<< "ui->Date_Show->text() :" << ui->Date_Show->text() ;
            QString Month_no = QDateTime::fromString(ui->Date_Show->text(), "dd-MM-yyyy").toString("MM");
            QString Year_no = (QDateTime::fromString(ui->Date_Show->text(), "dd-MM-yyyy")).toString("yyyy");

            qDebug() << __func__ << __LINE__<< "Month_no" << Month_no ;
            qDebug() << __func__ << __LINE__<< "Year_no :" << Year_no ;

            insert_command.append("select * from Daily_Data_Entry_Table where name='");
            insert_command.append(ui->Search_name->text());
            insert_command.append("' and date BETWEEN '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-");
            insert_command.append("01' AND '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-31'");

            qDebug() << __LINE__<< "insert_command :" << insert_command ;

            wordList = db_edit->get_all_customer_data(insert_command);
            qDebug() << __LINE__<< "All Customer Data :" << wordList ;
            insert_command.clear();
            wordList.append("");
            wordList.append("Total");

            insert_command.append("select SUM(Transfer) from Daily_Data_Entry_Table where name='");
            insert_command.append(ui->Search_name->text());
            insert_command.append("' and date BETWEEN '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-");
            insert_command.append("01' AND '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-31'");

            qDebug() << __LINE__<< "insert_command :" << insert_command ;
            wordList.append(db_edit->get_return_value(insert_command));
            insert_command.clear();

            insert_command.append("select SUM(Received) from Daily_Data_Entry_Table where name='");
            insert_command.append(ui->Search_name->text());
            insert_command.append("' and date BETWEEN '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-");
            insert_command.append("01' AND '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-31'");

            qDebug() << __LINE__<< "insert_command :" << insert_command ;
            wordList.append(db_edit->get_return_value(insert_command));
            insert_command.clear();

            insert_command.append("select SUM(Money_paid) from Daily_Data_Entry_Table where name='");
            insert_command.append(ui->Search_name->text());
            insert_command.append("' and date BETWEEN '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-");
            insert_command.append("01' AND '");
            insert_command.append(Year_no);
            insert_command.append("-");
            insert_command.append(Month_no);
            insert_command.append("-31'");

            qDebug() << __LINE__<< "insert_command :" << insert_command ;
            wordList.append(db_edit->get_return_value(insert_command));
            insert_command.clear();
            qDebug() << __LINE__<< "wordList :" << wordList ;

            //Table init
            labels << tr("Name") << tr("Date") << tr("Transfer") << tr("Received") << tr("Paid");
            qDebug() << __LINE__<< "labels :" << labels ;
            ColumnCount = labels.size();
            for(i = 0; i < ColumnCount; i++)
            {
                ui->Info_table->insertColumn(i);
            }
            ui->Info_table->setHorizontalHeaderLabels(labels);

            RowCount = wordList.size();
            qDebug() << __LINE__<< "circle :" << RowCount ;

            for(int i =0; i < (RowCount/5); i++)
            {
                qDebug() << __LINE__<< "i :" << i ;
                ui->Info_table->insertRow(i);

                ui->Info_table->setItem(i, 0, new QTableWidgetItem( wordList[ (5 * i) + 0] ));
                ui->Info_table->setItem(i, 1, new QTableWidgetItem( wordList[ (5 * i) + 1] ));
                ui->Info_table->setItem(i, 2, new QTableWidgetItem( wordList[ (5 * i) + 2] ));
                ui->Info_table->setItem(i, 3, new QTableWidgetItem( wordList[ (5 * i) + 3] ));
                ui->Info_table->setItem(i, 4, new QTableWidgetItem( wordList[ (5 * i) + 4] ));

            }
            //ui->Info_table->sortByColumn(0, Qt::AscendingOrder);
            //ui->Info_table->setSortingEnabled(true);

            ui->Info_table->show();
        }
        else
        {
            QMessageBox::critical(this,"Credentials Problem", "Please Give Proper Name","OK");
        }
    }

    delete ok;
    insert_command.clear();

}

void MainWindow::Delete_Table_row()
{
    int RowCount = ui->Info_table->rowCount();
    qDebug() << __LINE__ << "RowCount: " << RowCount;
    if(RowCount)
    {
        for(int i = (RowCount - 1); i >= 0 ; i--)
        {
            //qDebug() << __LINE__ << "i : " << i;
            ui->Info_table->removeRow(i);
        }
    }
}

void MainWindow::Delete_Table_Column()
{
    int ColumnCount = ui->Info_table->columnCount();
    qDebug() << __LINE__ << "ColumnCount: " << ColumnCount;
    if(ColumnCount)
    {
        for(int  i = (ColumnCount - 1); i >= 0; i--)
        {
            //qDebug() << __LINE__ << "i : " << i;
            ui->Info_table->removeColumn(i);
        }
    }
}


void MainWindow::Sync_For_A_Customer(QString Cust_name, QString cust_date)
{
    /*
     *  0 - dd
     *  1 - /
     *  2 - yy
    */
    //QStringList DateList;
    //TODO remove if not required date_consider
    //
    QString date_consider= "";

    QString insert_command = "";
    QStringList ret_value;
    //Get value of Last_Month_Bill
    //select Last_Month_Bill from Monthly_Data_Tracking_Table where name='Kailash' and month
    insert_command.clear();
    insert_command.append("select Last_Month_Bill from Monthly_Data_Tracking_Table where name='");
    insert_command.append(Cust_name);
    insert_command.append("' and month='");
    insert_command.append((QDateTime::fromString(cust_date, "dd-MM-yyyy")).toString("MM-yyyy"));
    insert_command.append("'");

    qDebug() << __LINE__<< "cust_date :" << cust_date ;

    qDebug() << __LINE__<< "date_consider :" << insert_command ;
    qDebug() << __LINE__<< "get_return_value :" << db_edit->get_return_value(insert_command) ;

    ret_value.append(db_edit->get_return_value(insert_command));

    if (  ret_value.contains("") )
    {
        ret_value.append("");
    }
    else
    {
        ret_value.append("1");
    }

    ret_value.append( db_edit->get_return_value(insert_command));
    //qDebug() << __LINE__<< "get_return_value :" << ret_value ;
    //qDebug() << __LINE__<< "get_return_value :" << ret_value[0] ;


    if(ret_value[0] == "")
    {
        QMessageBox::information(this, "Success", "New Customer no last month bill sync cancle","OK");
    }
    else
    {

        sync_data_for_a_month( Cust_name , cust_date );

     /*

        int month = ((QDateTime::fromString(cust_date, "dd-MM-yyyy")).toString("MM")).toInt(ok,10);
        int current_month = QDate::currentDate().month();
        qDebug() << __LINE__<< "month : " << month << " current_month : " << current_month;
        for (; month <= current_month;month++)
        {
            qDebug() << __LINE__<< "date_consider :" << cust_date ;

            qDebug() << __LINE__<< "date_consider :" << cust_date ;
            if(sync_data_for_a_month( Cust_name , cust_date ) )
            {
                break;
            }

            date_consider.clear();

        }*/

    }
}

int MainWindow::sync_data_for_a_month(QString Cu_Name, QString search_date)
{
    qDebug() << __LINE__<< "Enter into sync_data_for_a_month" ;
    QString insert_command = "";
    /*

    SELECT SUM(Transfer) FROM Daily_Data_ENTRY_TABLE where name='raj' and date BETWEEN '01-12-18' AND '31-12-18'
    SELECT SUM(Received) FROM Daily_Data_ENTRY_TABLE where name='raj' and date BETWEEN '01-12-18' AND '31-12-18'
    SELECT SUM(Money_paid) FROM Daily_Data_ENTRY_TABLE where name='raj' and date BETWEEN '01-12-18' AND '31-12-18'

    for every customer
        Entry :
        calculate   total taken         = sum of taken can in this month
                    total received      = sum of received can in this month
                    total paid          = sum of Money paid in this month
                    current month bill  = Total taken * (price for can in DB)
                    last month to be paid = take from DB
                    To be paid          = last month to be paid - total paid + current month bill
                                            return_list[5]+return_list[4] - return_list[2]
    */
    if(Cu_Name != "" )
    {
         /*
          * return_list[0] = Transfer
          * return_list[1] = Received
          * return_list[2] = Money_paid
          * return_list[3] = price for a can
          * return_list[4] = Current month bill
          * return_list[5] = last month to be paid magil dene
          * return_list[6] = current month to be paid
          * return_list[7] = Last_To_Pay
          */
        QString Month_no = (QDateTime::fromString(search_date, "dd-MM-yyyy")).toString("MM");
        QString Year_no = (QDateTime::fromString(search_date, "dd-MM-yyyy")).toString("yyyy");
        qDebug() << __LINE__<< "Month_no :" << Month_no ;
        qDebug() << __LINE__<< "Year_no :" << Year_no ;

        insert_command.append("SELECT SUM(Transfer) FROM Daily_Data_ENTRY_TABLE where name='");
        insert_command.append(Cu_Name);
        insert_command.append("' and date BETWEEN '");
        insert_command.append(Year_no);
        insert_command.append("-");
        insert_command.append(Month_no);
        insert_command.append("-");
        insert_command.append("01' AND '");
        insert_command.append(Year_no);
        insert_command.append("-");
        insert_command.append(Month_no);
        insert_command.append("-31'");

        qDebug() << __LINE__<< "insert_command :" << insert_command ;
        QStringList return_list = db_edit->get_return_value(insert_command);// return list value 0
        qCritical() << __LINE__ <<  "Total can taken return_list[0]: " << return_list;
        insert_command.clear();

        insert_command.append("SELECT SUM(Received) FROM Daily_Data_ENTRY_TABLE where name='");
        insert_command.append(Cu_Name);
        insert_command.append("' and date BETWEEN '");
        insert_command.append(Year_no);
        insert_command.append("-");
        insert_command.append(Month_no);
        insert_command.append("-");
        insert_command.append("01' AND '");
        insert_command.append(Year_no);
        insert_command.append("-");
        insert_command.append(Month_no);
        insert_command.append("-31'");

        qDebug() << __LINE__<< "insert_command :" << insert_command ;
        return_list.append( db_edit->get_return_value(insert_command));// return list value 1
        qCritical() << __LINE__ <<  "Total can received return_list[1] : " << return_list;
        insert_command.clear();

        insert_command.append("SELECT SUM(Money_paid) FROM Daily_Data_ENTRY_TABLE where name='");
        insert_command.append(Cu_Name);
        insert_command.append("' and date BETWEEN '");
        insert_command.append(Year_no);
        insert_command.append("-");
        insert_command.append(Month_no);
        insert_command.append("-");
        insert_command.append("01' AND '");
        insert_command.append(Year_no);
        insert_command.append("-");
        insert_command.append(Month_no);
        insert_command.append("-31'");

        qDebug() << __LINE__<< "insert_command :" << insert_command ;
        return_list.append( db_edit->get_return_value(insert_command));// return list value 2
        qCritical() << __LINE__ <<  "Total money paid in this month return_list[2]: " << return_list;
        insert_command.clear();

        //SELECT price FROM Customer_Info_Table where name='raj'
        insert_command.append("SELECT price FROM Customer_Info_Table where name='");
        insert_command.append(Cu_Name);
        insert_command.append("'");

        qDebug() << __LINE__<< "insert_command :" << insert_command ;
        return_list.append( db_edit->get_return_value(insert_command));// return list value 3
        qCritical() << __LINE__ <<  "Price For a can return_list[3] : " << return_list;
        insert_command.clear();
        qCritical() << __LINE__ <<  "return_list : " << return_list;

        bool*ok= NULL;
        return_list.append(QString::number( return_list[0].toInt(ok,10)*return_list[3].toInt(ok,10)));// return list value 4
        qCritical() << __LINE__ <<  "Total bill for the month return_list[4] : " << return_list;


        //SELECT EXISTS(SELECT 1 FROM Monthly_Data_Tracking_Table  WHERE Name='rakesh' and date='18-12-2018' ); asel tr 1 nasel tr 0
        insert_command.append("SELECT EXISTS(SELECT 1 FROM Monthly_Data_Tracking_Table WHERE name='");
        insert_command.append(Cu_Name);
        insert_command.append("' and Month='");
        insert_command.append(((QDateTime::fromString(search_date, "dd-MM-yyyy")).addMonths(-1)).toString("MM-yyyy"));
        insert_command.append("' );");
        qDebug() << __LINE__ << "insert_command : " << insert_command;

        QStringList return_value = db_edit->get_return_value(insert_command);
        qCritical() << __LINE__ <<  "data exists status : " << return_value[0];
        insert_command.clear();

        if(return_value[0].toInt(ok,10))
        {
            //SELECT To_Be_Paid_in_This_Month FROM Monthly_Data_Tracking_Table where name='someshwar' and Month='12-2018'
            insert_command.append("SELECT To_Be_Paid_in_This_Month FROM Monthly_Data_Tracking_Table where name='");
            insert_command.append(Cu_Name);
            insert_command.append("' and Month='");
            insert_command.append(((QDateTime::fromString(search_date, "dd-MM-yyyy")).addMonths(-1)).toString("MM-yyyy"));
            insert_command.append("' ");

            qDebug() << __LINE__<< "insert_command :" << insert_command ;
            return_list.append( db_edit->get_return_value(insert_command));
            qCritical() << __LINE__ <<  "data exists status : " << return_list;
            insert_command.clear();

        }else
        {
            QMessageBox::information(this, "Permission", "last month To Pay value is not present default 0 will be taken","OK");
            return_list.append("0");
        }
        return_list.append( QString::number(((return_list[5].toInt(ok,10)+return_list[4].toInt(ok,10)) - return_list[2].toInt(ok,10))) );


        if((return_list[5].toInt(ok,10) - return_list[2].toInt(ok,10)) < 0 )
        {
            return_list.append("0");
        }
        else
        {
            return_list.append( QString::number(((return_list[5].toInt(ok,10)) - return_list[2].toInt(ok,10))) );
        }
        //SELECT EXISTS(SELECT 1 FROM Monthly_Data_Tracking_Table  WHERE Name='rakesh' and date='18-12-2018' ); asel tr 1 nasel tr 0
        insert_command.append("SELECT EXISTS(SELECT 1 FROM Monthly_Data_Tracking_Table WHERE name='");
        insert_command.append(Cu_Name);
        insert_command.append("' and Month='");
        insert_command.append( Month_no );
        insert_command.append("-");
        insert_command.append(Year_no);
        insert_command.append("' );");
        qDebug() << __LINE__ << "insert_command : " << insert_command;

        return_value = db_edit->get_return_value(insert_command);
        qCritical() << __LINE__ <<  "data exists status : " << return_value[0];
        insert_command.clear();

        if(return_value[0].toInt(ok,10))
        {
            //update
            //UPDATE Monthly_Data_Tracking_Table set Total_Transfer='45',Total_Received='45',Paid_In_Current_month='45',Current_Month_Bill='45',
            //To_Be_Paid_In_This_Month = '1500', Last_Month_Bill = '0', Last_To_Pay = '0' where Name='rakesh' and month='12-2018';
            insert_command.append("UPDATE Monthly_Data_Tracking_Table set Total_Transfer='");
            insert_command.append(return_list[0]);
            insert_command.append("', Total_Received='");
            insert_command.append(return_list[1]);
            insert_command.append("', Paid_In_Current_month='");
            insert_command.append(return_list[2]);
            insert_command.append("', Current_Month_Bill='");
            insert_command.append(return_list[4]);
            insert_command.append("', To_Be_Paid_In_This_Month = '");
            insert_command.append(return_list[6]);
            insert_command.append("', Last_Month_Bill = '");
            insert_command.append(return_list[5]);
            insert_command.append("', Last_To_Pay = '");
            insert_command.append(return_list[7]);
            insert_command.append("' where Name='");
            insert_command.append(Cu_Name);
            insert_command.append("' and Month='");
            insert_command.append(Month_no);
            insert_command.append("-");
            insert_command.append(Year_no);
            insert_command.append("'");
            qDebug() << __LINE__<< "insert_command :" << insert_command ;

        }else
        {
            //INSERT into Monthly_Data_Tracking_Table (ID,Name,Month,Total_Transfer,Total_Received,Paid_In_Current_month,Current_Month_Bill,To_Be_Paid_In_This_Month)
            //values ((select id from Customer_info_table where name="rakesh"),'rakesh', '12-2018', '45', '46','900','1600', '1500')

            insert_command.append("INSERT into Monthly_Data_Tracking_Table (ID,Name,Month,Total_Transfer,Total_Received,Paid_In_Current_month,Current_Month_Bill,To_Be_Paid_In_This_Month,Last_Month_Bill,Last_To_Pay) values ((select id from Customer_info_table where name='");
            insert_command.append(Cu_Name);
            insert_command.append("'), '");
            insert_command.append(Cu_Name);
            insert_command.append("', '");
            insert_command.append(Month_no);
            insert_command.append("-");
            insert_command.append(Year_no);
            insert_command.append("', '");
            insert_command.append(return_list[0]);
            insert_command.append("', '");
            insert_command.append(return_list[1]);
            insert_command.append("', '");
            insert_command.append(return_list[2]);
            insert_command.append("', '");
            insert_command.append(return_list[4]);
            insert_command.append("', '");
            insert_command.append(return_list[6]);
            insert_command.append("', '");
            insert_command.append(return_list[5]);
            insert_command.append("', '");
            insert_command.append(return_list[7]);
            insert_command.append("')");
            qDebug() << __LINE__<< "insert_command :" << insert_command ;

        }
        QSqlError err = db_edit->Execute_query(insert_command);
        if (err.type() != QSqlError::NoError)
        {
            qCritical() << __LINE__ <<  "monthly sync fail for : " << Cu_Name <<  err.text() << "number" << err.number() << err.type();
            QMessageBox::critical(this,"DataBase Error", "Query for sync fail data not saved","close");
            return -1;
        }else
        {
            //QMessageBox::information(this, "Success", "Customer Data Sync successfully","OK");
            insert_command.clear();
            delete ok;
            return 0;
        }
    }else
    {
        QMessageBox::information(this, "Credential Problem", "Please Provide Name","OK");
        qDebug() << __LINE__<< "Exit from sync_data_for_a_month" ;
        return -1;
    }
    qDebug() << __LINE__<< "Exit from sync_data_for_a_month" ;
    return 0;
}


void MainWindow::on_SAFSD_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Data For Single Date")
    {
        ui->Date_Show->setVisible(true);
        ui->label_11->setVisible(false);
        ui->Search_name->setVisible(false);
        ui->Show_details->setText("Show Details");
    }else if(arg1 == "Customer Monthly Data")
    {
        ui->label_11->setVisible(true);
        ui->Search_name->setVisible(true);
        ui->Date_Show->setVisible(true);
        ui->Show_details->setText("Show Details");

    }else if(arg1 == "All Customer Information")
    {
        ui->label_11->setVisible(false);
        ui->Search_name->setVisible(false);
        ui->Date_Show->setVisible(false);
        ui->Show_details->setText("Show Details");
    }
    else if(arg1 == "Sync For All")
    {
        ui->Date_Show->setVisible(true);
        ui->label_11->setVisible(false);
        ui->Search_name->setVisible(false);
        ui->Show_details->setVisible(true);
        ui->Show_details->setText("Sync Month");
    }else if(arg1 == "Sync For Customer")
    {
        ui->Date_Show->setVisible(true);
        ui->label_11->setVisible(true);
        ui->Search_name->setVisible(true);
        ui->Show_details->setVisible(true);
        ui->Show_details->setText("Sync Customer");
    }
    else if(arg1 == "Customer Monthly Report")
    {
        ui->label_11->setVisible(true);
        ui->Search_name->setVisible(true);
        ui->Date_Show->setVisible(true);
        ui->Show_details->setText("Show Details");
    }
    else if(arg1 == "Monthly report")
    {
        ui->label_11->setVisible(false);
        ui->Search_name->setVisible(false);
        ui->Date_Show->setVisible(true);
        ui->Show_details->setText("Show Details");
    }

    ui->stackedWidget->setCurrentWidget(ui->Overview);
}





QString MainWindow::Update_Query_For_Daily_data_Table()
{
    //update
    //update Daily_Data_Entry_Table set Transfer='75', Received='85', Money_paid='500' where ID='2' and date='18-12-2018';
    qDebug() << "date :" << ui->Set_date->text();
    QString Command;
    Command.append("update Daily_Data_Entry_Table set Transfer='");
    Command.append(ui->value_taken->text());
    Command.append("', Received='");
    Command.append(ui->value_return->text());
    Command.append("', Money_paid='");
    Command.append(ui->value_money->text());
    Command.append("' where name='");
    Command.append(ui->Value_name->text());
    Command.append("' and date='");
    Command.append((QDateTime::fromString(ui->Set_date->text(), "dd-MM-yyyy")).toString("yyyy-MM-dd"));
    Command.append("' ;");
    return Command;
}

QString MainWindow::Insart_Query_For_Daily_data_Table()
{
    //insert
    //insert into Daily_Data_Entry_Table (ID,Name,DATE,Transfer,Received,Money_paid) values ((select id from Customer_info_table where name="rakesh"), '20-12-2018', '4', '5', '1500');
    qDebug() << "date :" << ui->Set_date->text();
    QString Command;
    Command.append("INSERT into Daily_Data_Entry_Table (ID,Name,DATE,Transfer,Received,Money_paid) values ((select id from Customer_info_table where name='");
    Command.append(ui->Value_name->text());
    Command.append("'), '");
    Command.append(ui->Value_name->text());
    Command.append("', '");
    Command.append((QDateTime::fromString(ui->Set_date->text(), "dd-MM-yyyy")).toString("yyyy-MM-dd"));
    Command.append("', '");
    Command.append(ui->value_taken->text());
    Command.append("', '");
    Command.append(ui->value_return->text());
    Command.append("', '");
    Command.append(ui->value_money->text());
    Command.append("');");
    return Command;
}

void MainWindow::on_Session_Exit_clicked()
{
    this->close();
}

void MainWindow::on_Destroy_session_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Welcome);
}

void MainWindow::on_testcombo_clicked()
{
   qDebug() << __LINE__ << ui->Select_Action->currentData();
   qDebug() << __LINE__ << ui->Select_Action->currentData();
}




/*
    QMessageBox msgBox;
    msgBox.setText(tr("Confirm?"));
    QAbstractButton* pButtonYes = msgBox.addButton(tr("Yeah!"), QMessageBox::YesRole);
    msgBox.addButton(tr("Nope"), QMessageBox::NoRole);

    msgBox.exec();

    if (msgBox.clickedButton()==pButtonYes) {
        qDebug() << "date :" << QDate::currentDate().toString();
        qDebug() << "date :" << QDate::currentDate().toString("dd-MM-yyyy");
        qDebug() << "Time :" << QTime::currentTime().toString("hh:mm:ss");
        qDebug() << "Time :" << QTime::currentTime().toString();
        //Execute command
    }*/
/*QMessageBox msgBox;
msgBox.setWindowTitle("title");
msgBox.setText("Question");
msgBox.setStandardButtons(QMessageBox::Yes);
msgBox.addButton(QMessageBox::No);
msgBox.setDefaultButton(QMessageBox::No);
if(msgBox.exec() == QMessageBox::Yes){
  // do something
}else {
  // do something else
}*/

void MainWindow::on_Select_Action_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Update")
    {
        qDebug() << __LINE__ << "switch to : " << arg1;
        ui->To_pay->setVisible(false);
        ui->label_13->setVisible(false);
        ui->Save_Data->setText("Update");
    }
    else if(arg1 == "Add")
    {
        qDebug() << __LINE__ << "switch to : " << arg1;
        ui->To_pay->setVisible(true);
        ui->label_13->setVisible(true);
        ui->Save_Data->setText("Add");
    }
    else if(arg1 == "Delete")
    {
        qDebug() << __LINE__ << "switch to : " << arg1;
        ui->To_pay->setVisible(false);
        ui->label_13->setVisible(false);
        ui->Save_Data->setText("Delete");
    }

}

void MainWindow::on_SU_Record_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Save Record")
    {
        ui->Save_daily_data->setText("Save");
    }
    else if(arg1 == "Update Record")
    {
        ui->Save_daily_data->setText("Update");
    }
}







void MainWindow::on_CB_gen_bill_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Generate Bill For A Customer")
    {
        ui->label_14->setVisible(true);
        ui->Name_for_bill->setVisible(true);
    }
    else if(arg1 == "Generate Bill For All")
    {
        ui->label_14->setVisible(false);
        ui->Name_for_bill->setVisible(false);
    }
}

void MainWindow::on_btn_gen_bill_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_bill);
}

void MainWindow::on_btn_gen_pdf_clicked()
{
    qDebug() << __LINE__ << "Enter in generate PDF";

    //Toget data from DB
    // command
    //Date on bill
    QStringList  wordList;
    QString insert_command = "";
    QString Bill_Date = ui->Date_for_bill->text();


    bool *ok = NULL ;
    //
    // No of customers
    int rounds ;
    int no_of_bill;

    //File in which bill will be save
    QString FileName;
    if(ui->folder->text() == "")
    {
        FileName.append(QDir::currentPath());
    }
    else
    {
        FileName.append(ui->folder->text());
    }

    QDateTime last_month_date = QDateTime::fromString(Bill_Date, "dd-MM-yyyy").addMonths(-1);
    qDebug() << __LINE__ << "last_month_date : " << last_month_date;






    switch (ui->CB_gen_bill->currentIndex()) {
    case 0:
        qDebug() << __LINE__ << "Swich case 0 bill_generate_for_single_customer";

        /*
         * date
         * name
         * rate
         * Total transfer cans
         * current month bill
         * last month bill
         * this month paid
         * last to pay
         * total to pay
         *
         *
         * get from ui
         *  Date
         * name
         *
         * //select * from Daily_Data_Entry_Table where name='rakesh' and date BETWEEN '1-11-2018' AND '31-11-2018'
         *select * from Monthly_Data_Tracking_Table where name='Rajesh' and Month='08-2019'
         * select * from Monthly_Data_Tracking_Table where Month='08-2019'
         * */
        qDebug() << __LINE__ << "Name for bill" << ui->Name_for_bill->text();
        qDebug() << __LINE__ << "Date for bill" << Bill_Date;

        FileName.append("/");
        FileName.append(ui->Name_for_bill->text());
        FileName.append((QDateTime::fromString(last_month_date.toString("dd-MM-yyyy"), "dd-MM-yyyy")).toString("-MM-yyyy"));
        FileName.append(".pdf");
        qDebug() << __LINE__ << "FileName : " << FileName;

        if (ui->Name_for_bill->text() == "")
        {
            //show message boox * credentials are necessary
            QMessageBox::critical(this, "Credential Error","please provide name and date properly ","OK");
            return ;
        }

        insert_command.append("select * from Monthly_Data_Tracking_Table where name='");
        insert_command.append(ui->Name_for_bill->text());
        insert_command.append("' and Month='");
        insert_command.append(((QDateTime::fromString(Bill_Date, "dd-MM-yyyy")).addMonths(-1)).toString("MM-yyyy"));
        insert_command.append("'");
        qDebug() << __LINE__<< "insert_command :" << insert_command ;

        wordList = db_edit->monthly_data_of_cust_bill(insert_command);
        qCritical() << __LINE__<<  "data exists status : " << wordList;

        wordList.append(Bill_Date);

        //Get price for cust
        //select Price from Customer_Info_Table where name='Kailash'
        insert_command.clear();
        insert_command.append("select Price from Customer_Info_Table where name='");
        insert_command.append(ui->Name_for_bill->text());
        insert_command.append("'");
        wordList.append(db_edit->get_return_value(insert_command));


        qCritical() << __LINE__<<  "data exists status : " << wordList;

        qDebug() << __LINE__<< "Month :" << QDate::longMonthName(((QDateTime::fromString(last_month_date.toString("dd-MM-yyyy"), "dd-MM-yyyy")).toString("MM")).toInt(ok,10));
        wordList.append( QDate::longMonthName((QDateTime::fromString(last_month_date.toString("dd-MM-yyyy"), "dd-MM-yyyy")).toString("MM").toInt(ok,10)));


        qCritical() << __LINE__<<  "data exists status : " << wordList;
        Bill->bill_generate_for_single_customer(wordList,FileName);
        break;
    case 1:
        {
            qDebug() << __LINE__ << "Swich case 1 bill_generate_for_all_customer";
            insert_command.append("select * from Monthly_Data_Tracking_Table where Month='");
            insert_command.append(((QDateTime::fromString(ui->Date_for_bill->text(), "dd-MM-yyyy")).addMonths(-1)).toString("MM-yyyy"));
            insert_command.append("'");

            qDebug() << __LINE__<< "insert_command :" << insert_command ;

            wordList = db_edit->monthly_data_of_cust_bill(insert_command);
            qDebug() << __LINE__<<  "data exists status : " << wordList;
            qDebug() << __LINE__<<  "size : " << wordList.size();
            rounds = 9;
            qDebug() << __LINE__<<  "rounds : " << rounds;
            no_of_bill =  wordList.size() / rounds;
            qDebug() << __LINE__<<  "no_of_bill : " << no_of_bill;

            QStringList local_data_carrier;

            FileName.append("/Bills-");
            //Kailsh-08-2019.pdf Bills-08-2019.pdf
            FileName.append((QDateTime::fromString(ui->Date_for_bill->text(), "dd-MM-yyyy")).toString("MM-yyyy"));
            FileName.append(".pdf");
            qDebug() << __LINE__<<  "FileName : " << FileName;



            for(int i = 1; i <= no_of_bill; i++ )
            {
                for(int j = 0; j < rounds; j++)
                {
                    local_data_carrier.append(wordList[0]);
                    wordList.removeFirst();
                }

                qDebug() << __LINE__<<  "local_data_carrier : " << local_data_carrier;


                local_data_carrier.append(Bill_Date);

                //select Price from Customer_Info_Table where name='Kailash'
                insert_command.clear();
                insert_command.append("select Price from Customer_Info_Table where name='");
                insert_command.append(local_data_carrier[0]);
                insert_command.append("'");
                local_data_carrier.append(db_edit->get_return_value(insert_command));
                qDebug() << __LINE__<<  "data exists status : " << local_data_carrier;

                local_data_carrier.append( QDate::longMonthName((QDateTime::fromString(last_month_date.toString("dd-MM-yyyy"), "dd-MM-yyyy")).toString("MM").toInt(ok,10)));
                qDebug() << __LINE__<<  "local_data_carrier : " << local_data_carrier;

            }
            Bill->bill_generate_for_single_customer(local_data_carrier,FileName);



            break;
        }
    default:
        qDebug() << __LINE__ << "Swich case Default";
        break;
    }
    QString message_show = "Your pdf is saved at : ";
    message_show.append(FileName);
    QMessageBox::information(this,"Bills", message_show,"close");

    qDebug() << __LINE__ << "Exit from generate pdf";

    return;

}


void MainWindow::on_btn_GO_HOME_clicked()
{
    qDebug() << __LINE__ << "Enter in on_btn_GO_HOME_clicked";
    ui->stackedWidget->setCurrentWidget(ui->Welcome);
    qDebug() << __LINE__ << "Exit from on_btn_GO_HOME_clicked";
}

void MainWindow::on_SaveToFolder_clicked()
{
    QString FolderName = QFileDialog::getExistingDirectory(this,"open a folder",QDir::homePath());
    ui->folder->setText(FolderName);
}
