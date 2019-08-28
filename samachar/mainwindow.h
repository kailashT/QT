#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCompleter>

#include "configuration.h"
#include "sqlitedb.h"
#include "create_bill.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Save_daily_data_clicked();

    void on_btn_add_user_clicked();

    void on_btn_daily_data_clicked();

    void on_btn_back_clicked();

    void UpdateTime();

    void on_btn_overview_clicked();

    void on_btn_add_user_pressed();

    void on_Destroy_session_clicked();

    void on_Show_details_clicked();

    void on_Session_Exit_clicked();

    void on_Destroy_session_2_clicked();

    void on_testcombo_clicked();

    void on_Save_Data_clicked();

    void check_for_database(void);

    void on_Select_Action_currentIndexChanged(const QString &arg1);

    void on_SU_Record_currentIndexChanged(const QString &arg1);


    void on_SAFSD_currentIndexChanged(const QString &arg1);

    QString Update_Query_For_Daily_data_Table();

    QString Insart_Query_For_Daily_data_Table();

    int sync_data_for_a_month(QString,QString);

    void Sync_For_A_Customer(QString,QString);
    void Delete_Table_row();
    void Delete_Table_Column();

    void on_CB_gen_bill_currentIndexChanged(const QString &arg1);

    void on_btn_gen_bill_clicked();

    void on_btn_gen_pdf_clicked();

    void on_btn_GO_HOME_clicked();

    void on_SaveToFolder_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *digital_timer;
    sqlitedb *db_edit;
    QStringList wordListname ;
    create_bill *Bill;
};

#endif // MAINWINDOW_H
