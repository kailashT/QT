#include "sqlitedb.h"

sqlitedb::sqlitedb()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("Mydbcheck");

   if (!m_db.open())
   {
      qDebug() << "Error: connection with database fail. " << m_db.lastError();
   }
   else
   {
      qDebug() << "Database: connection ok. " << m_db.lastError();
   }
}

sqlitedb::~sqlitedb()
{
    m_db.close();
}

QSqlError sqlitedb::Add_User()
{
    qDebug() << "Database: you are in Add_User.";
    return m_db.lastError();
}

QSqlError sqlitedb::Delete_User()
{
    qDebug() << "Database: you are in Delete_User.";
    return m_db.lastError();
}

QSqlError sqlitedb::Update_user()
{
    qDebug() << "Database: you are in Update_user.";
    return m_db.lastError();
}

QSqlError sqlitedb::Get_user()
{
    qDebug() << "Database: you are in Get_user.";
    return m_db.lastError();
}

QStringList sqlitedb::Get_Table_Names()
{
    QStringList tables = m_db.tables();
    return tables;
}

QSqlError sqlitedb::If_Table_Exists(QByteArray tablename)
{
    QStringList tables = m_db.tables();
    if (tables.contains(tablename, Qt::CaseInsensitive))
    {
        qDebug() << "Database: table Found.";
    }
    else
        qDebug() << "Database: table Not Found.";
        if (tablename == "books")
            Create_table(BOOKTABLE);
        else if (tablename == "authors")
            Create_table(AUTHORTABLE);


    return QSqlError();
}

QSqlError sqlitedb::Create_table(QByteArray qtct) //query to create table
{
    QSqlQuery q;
    if (!q.exec(QLatin1String(qtct)))
        return q.lastError();
    qDebug() << "Database: create table using query : " << qtct;
    return QSqlError();
}
