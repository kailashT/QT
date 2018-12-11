#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <QDebug>
#include <QtSql>
#include <QByteArray>


#define BOOKTABLE "create table books(id integer primary key, title varchar, author integer, genre integer, year integer, rating integer)"
#define AUTHORTABLE "create table authors(id integer primary key, name varchar, birthdate date)"



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
    QSqlError If_Table_Exists(QByteArray);
    QSqlError Create_table(QByteArray);


private:
    QSqlDatabase m_db;
};

#endif // SQLITEDB_H
