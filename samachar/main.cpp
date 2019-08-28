#include "mainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


/*

int main(int argc, char *argv[])
{
   qDebug() << (0 & 1);
   return 0;
}
*/
