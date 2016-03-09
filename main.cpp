#include "mainwindow.h"
#include "dbconfigdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString line{};
    if(!DBConfigDialog::readFromFile(line))
        return 0;

    if(!line.isEmpty()){
        const QStringList parameters = line.split(";");
        Database::setParameters(parameters.at(0), parameters.at(1).toInt(), parameters.at(2), parameters.at(3), parameters.at(4));

        if(parameters.at(0)=="localhost") Database::isLocal = true;
        else Database::isLocal = false;

        // check connection
        if(Database::connectToDatabase()){
             MainWindow::isDatabase = true;
             DBConfigDialog::user = parameters.at(3);
             DBConfigDialog::password = parameters.at(4);
             DBConfigDialog::currentAddress = parameters.at(0);
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
