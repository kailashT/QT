#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <QDebug>
#include <QtSql>
#include <QByteArray>

// queries
#define BOOKTABLE "create table books(id integer primary key, title varchar, author integer, genre integer, year integer, rating integer)"
#define AUTHORTABLE "create table authors(id integer primary key, name varchar, birthdate date)"


#define CUSTOMERTABLE "CREATE TABLE IF NOT EXISTS Customer_Info_Table (ID integer PRIMARY KEY autoincrement, Name text NOT NULL UNIQUE, Address text, MoNumber text NOT NULL UNIQUE, MoNumber2 text,Price integer NOT NULL, CONSTRAINT uiocount UNIQUE (Name, MoNumber))"
#define DAILY_DATA_ENTRY_TABLE "CREATE TABLE `Daily_Data_Entry_Table` (`ID`	INTEGER NOT NULL, 'Name' text NOT NULL, `DATE`	date NOT NULL,`Transfer`	INTEGER,`Received`	INTEGER,`Money_paid`	INTEGER);"
#define MONTHLY_DATA_TRACKING_TABLE "CREATE TABLE `Monthly_Data_Tracking_Table` (`ID`	INTEGER NOT NULL, 'Name' text NOT NULL, `Month`	date NOT NULL, `Total_Transfer`	INTEGER, `Total_Received`	INTEGER, `Paid_In_Current_Month`	INTEGER, `Current_Month_Bill`	INTEGER, `To_Be_Paid_In_This_Month`	INTEGER, `Last_Month_Bill`	INTEGER, `Last_To_Pay`	INTEGER);"


/*
 * ID integer PRIMARY KEY autoincrement
 * Name text NOT NULL
 * Address text
 * MoNumber text NOT NULL UNIQUE
 * MoNumber2 text
 * Price integer NOT NULL
 * CONSTRAINT uiocount UNIQUE (MoNumber)
 *
 * */

//DataBase
#define DBTYPE "QSQLITE"
#define DBNAME "Mydbcheck.db"


class sqlitedb
{
public:
    sqlitedb();
    ~sqlitedb();
    QSqlError Add_User();
    QSqlError Delete_User();
    QSqlError Update_user();
    QSqlError Get_user();
    QStringList Get_Table_Names();
    int If_Table_Exists(QString);
    QSqlError Create_table(QString);
    QSqlError Execute_query(QString);
    QStringList Get_names();
    QStringList Get_Mobile();
    QStringList get_return_value(QString);
    QStringList get_table_data(QString );
    QStringList get_all_customer_data(QString );
    QStringList Data_For_Single_Day(QString );
    QStringList Data_For_Monthly_Report(QString );
    QStringList monthly_data_of_cust_bill(QString );


private:
    QSqlDatabase m_db;
};

#endif // SQLITEDB_H


/*
#define CUSTOMER "CREATE TABLE IF NOT EXISTS Customers (ID integer PRIMARY KEY autoincrement, Name text NOT NULL UNIQUE, Address text NOT NULL, MoNumber text NOT NULL UNIQUE, MoNumber2 text NULL,PRICE integer NOT NULL, CONSTRAINT uiocount UNIQUE (Name,MoNumber));"
#define DAILY_DATA_ENTRY_TABLE "CREATE TABLE Daily_Data_ENTRY_TABLE (ID	INTEGER NOT NULL, DATE	date NOT NULL, Transfer INTEGER, Received INTEGER,Money_paid INTEGER);"
#define DAILY_DATA_ENTRY_TABLE2 "CREATE TABLE `Daily_Data_ENTRY_TABLE` (`ID`	INTEGER NOT NULL,`DATE`	date NOT NULL,`Transfer`	INTEGER,`Received`	INTEGER,`Money_paid`	INTEGER);"

*/

/*
 * CREATE TABLE `Monthly_Data_Tracking_Table` (
    `ID`	INTEGER NOT NULL,
    `Month`	INTEGER NOT NULL,
    `Total_Transfer`	INTEGER,
    `Total_Received`	INTEGER,
    `Paid_In_Current_Month`	INTEGER,
    `Current_Month_Bill`	INTEGER,
    `To_Be_Paid_In_This_Month`	INTEGER
);
 * */

//select id from Customer_info_table where name="raj"
//update Customer_Info_Table set address='pune' where name='rakesh'
//ADD data to DAILY_DATA_ENTRY_TABLE
//           INSERT INTO Daily_Data_ENTRY_TABLE (ID,DATE,Transfer,Received,Money_paid) VALUES ('15','18-12-2018','5','2','50')
//Selection of data
//         <> select * from Daily_Data_ENTRY_TABLE where `date` >= '15-12-2018' and `date` <= '18-12-2018'
//          SELECT * FROM mytalbe WHERE date BETWEEN '2014-10-09' AND '2014-10-10'
//SUM of column data from date to date
//           SELECT SUM(Transfer) FROM Daily_Data_ENTRY_TABLE where `date` >= '15-12-2018' and `date` <= '18-12-2018'
//           select Transfer from Daily_Data_ENTRY_TABLE where ID=17
//         select Transfer from Daily_Data_ENTRY_TABLE where ID=17
//         select Transfer from Daily_Data_ENTRY_TABLE where ID=17

//INSERT INTO Monthly_Data_Tracking_Table (ID,Month,Total_Transfer,Total_Received,Paid_In_Current_Month,Current_Month_Bill,To_Be_Paid_In_This_Month) VALUES ( (select id from Customer_info_table where name="rakesh"),'12-2018',null,null,null,null,'1200')
/*
 * SELECT EXISTS(SELECT 1 FROM Monthly_Data_Tracking_Table WHERE id='50' and month='12-2018' );
update Monthly_Data_Tracking_Table set To_Be_Paid_In_This_Month = '1500' where id='50' and month='12-2018';
 * */
//insert into Monthly_Data_Tracking_Table (id,month,To_Be_Paid_In_This_Month) values ((select id from Customer_info_table where name="rakesh"), '12-2018', '1500');


//SELECT EXISTS(SELECT 1 FROM Monthly_Data_Tracking_Table WHERE id='50' and month='12-2018' );
