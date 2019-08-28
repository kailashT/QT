#include "sqlitedb.h"

sqlitedb::sqlitedb()
{
    m_db = QSqlDatabase::addDatabase(DBTYPE);
    m_db.setDatabaseName(DBNAME);

   if (!m_db.open())
   {
      qDebug() << __LINE__ << "Error: connection with database fail. " << m_db.lastError();
   }
   else
   {
      qDebug() << __LINE__ << "Database: connection ok. " << m_db.lastError();
   }

}


sqlitedb::~sqlitedb()
{
    m_db.close();
}

QSqlError sqlitedb::Add_User()
{
    qDebug() << __LINE__ << "Database: you are in Add_User.";
    return m_db.lastError();
}

QSqlError sqlitedb::Delete_User()
{
    qDebug() << __LINE__ << "Database: you are in Delete_User.";
    return m_db.lastError();
}

QSqlError sqlitedb::Update_user()
{
    qDebug() << __LINE__ << "Database: you are in Update_user.";
    return m_db.lastError();
}

QSqlError sqlitedb::Get_user()
{
    qDebug() << __LINE__ << "Database: you are in Get_user.";
    return m_db.lastError();
}

QStringList sqlitedb::Get_Table_Names()
{
    QStringList tables = m_db.tables();
    return tables;
}

int sqlitedb::If_Table_Exists(QString tablename)
{
    QStringList tables = m_db.tables();
    if (tables.contains(tablename, Qt::CaseInsensitive))
    {
        qDebug() << __LINE__ << "Database: table Found.";
        return 0;
    }
    else
    {
        qDebug() << __LINE__ << "Database: table Not Found.";
        return -1;
    }
}

QSqlError sqlitedb::Create_table(QString qtct) //query to create table
{
    QSqlQuery q;
    if (!q.exec(qtct))
        return q.lastError();
    qDebug() << __LINE__ << "Database: create table using query : " << qtct;
    return QSqlError();
}

QSqlError sqlitedb::Execute_query(QString command)
{
    QSqlQuery query;
    if (!query.exec(command))
        return query.lastError();
    qDebug() << __LINE__ << "Database: query Executed: " << command;
    return QSqlError();
}

QStringList sqlitedb::Get_names()
{
    QStringList wordList;
    QSqlQuery q;
    q.exec(QLatin1String("select Name from Customer_Info_Table"));
    while (q.next())
    {
            wordList << q.value(0).toString();
    }
    return wordList;
}

QStringList sqlitedb::Get_Mobile()
{
    QStringList wordList;
    QSqlQuery q;
    q.exec(QLatin1String("select MoNumber from Customer_Info_Table"));
    while (q.next())
    {
            wordList << q.value(0).toString();
    }
    return wordList;
}

QStringList sqlitedb::get_return_value(QString Qexecute)
{
    QStringList wordList;
    QSqlQuery q;
    q.exec(Qexecute);
    while (q.next())
    {
            wordList << q.value(0).toString();
    }
    qDebug() << "get_return_value :" << wordList;
    return wordList;
}
//data for single day
QStringList sqlitedb::get_table_data(QString tabla)
{
    QStringList wordList;
    QSqlQuery mostrar;
    mostrar.prepare(tabla);

    if(mostrar.exec())
        qDebug() << "ok mostarr ";
    else
        qDebug() << "ERROR"<< mostrar.lastError();

    wordList.append("");
    while (mostrar.next())
    {
        wordList.append(mostrar.value(0).toString());
        wordList.append(mostrar.value(1).toString());
        wordList.append(mostrar.value(2).toString());
        wordList.append(mostrar.value(3).toString());
        wordList.append(mostrar.value(4).toString());
        qDebug() << "mostrar.value(1).toString()"<< mostrar.value(0).toString();
        qDebug() << "mostrar.value(1).toString()"<< mostrar.value(1).toString();
        qDebug() << "mostrar.value(2).toString()"<<mostrar.value(2).toString();
        qDebug() << "mostrar.value(1).toString()"<< mostrar.value(1).toString();

    }
    return wordList;
}

// to show all customer data
QStringList sqlitedb::get_all_customer_data(QString tabla)
{
    QStringList wordList;
    QSqlQuery mostrar;
    mostrar.prepare(tabla);

    if(mostrar.exec())
        qDebug() << "ok mostarr ";
    else
        qDebug() << "ERROR"<< mostrar.lastError();

    while (mostrar.next())
    {
        //wordList.append(mostrar.value(0).toString());
        wordList.append(mostrar.value(1).toString());
        wordList.append(mostrar.value(2).toString());
        wordList.append(mostrar.value(3).toString());
        wordList.append(mostrar.value(4).toString());
        wordList.append(mostrar.value(5).toString());
    }
    return wordList;
}



//data for single day
QStringList sqlitedb::Data_For_Single_Day(QString tabla)
{
    QStringList wordList;
    QSqlQuery mostrar;
    mostrar.prepare(tabla);

    if(mostrar.exec())
        qDebug() << "ok mostarr ";
    else
        qDebug() << "ERROR"<< mostrar.lastError();


    while (mostrar.next())
    {
        //wordList.append(mostrar.value(0).toString());
        wordList.append(mostrar.value(1).toString());
        wordList.append(mostrar.value(2).toString());
        wordList.append(mostrar.value(3).toString());
        wordList.append(mostrar.value(4).toString());
        wordList.append(mostrar.value(5).toString());
    }

    return wordList;
}

QStringList sqlitedb::Data_For_Monthly_Report(QString tabla)
{
    QStringList wordList;
    QSqlQuery mostrar;
    mostrar.prepare(tabla);

    if(mostrar.exec())
        qDebug() << "ok mostarr ";
    else
        qDebug() << "ERROR"<< mostrar.lastError();


    while (mostrar.next())
    {
        //wordList.append(mostrar.value(0).toString());
        wordList.append(mostrar.value(1).toString());
        wordList.append(mostrar.value(2).toString());
        wordList.append(mostrar.value(3).toString());
        wordList.append(mostrar.value(4).toString());
        wordList.append(mostrar.value(5).toString());
        wordList.append(mostrar.value(6).toString());
        wordList.append(mostrar.value(7).toString());
        wordList.append(mostrar.value(8).toString());//last month bill
        wordList.append(mostrar.value(9).toString());// last to pay
    }

    return wordList;

}

QStringList sqlitedb::monthly_data_of_cust_bill(QString tabla)
{
    QStringList wordList;
    QSqlQuery mostrar;
    mostrar.prepare(tabla);

    if(mostrar.exec())
        qDebug() << "ok mostarr query executed success ";
    else
        qDebug() << "ERROR"<< mostrar.lastError();


    while (mostrar.next())
    {
       // wordList.append(mostrar.value(0).toString());
        wordList.append(mostrar.value(1).toString());
        wordList.append(mostrar.value(2).toString());
        wordList.append(mostrar.value(3).toString());
        wordList.append(mostrar.value(4).toString());
        wordList.append(mostrar.value(5).toString());
        wordList.append(mostrar.value(6).toString());
        wordList.append(mostrar.value(7).toString());
        wordList.append(mostrar.value(8).toString());//last month bill
        wordList.append(mostrar.value(9).toString());// last to pay

    }

    return wordList;

}


