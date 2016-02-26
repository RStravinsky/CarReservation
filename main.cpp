#include "mainwindow.h"
#include "dbconfigdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBConfigDialog dbConfig(true);
    //if(dbConfig.exec()==DBConfigDialog::Rejected)
    //        return a.exec();
    MainWindow w;
    w.show();

    return a.exec();
}
