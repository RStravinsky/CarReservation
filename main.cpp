#include "mainwindow.h"
#include "dbconfigdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile initFile(QDir::currentPath()+"/init.txt");
    if (!initFile.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox(QMessageBox::Critical, QString("Błąd!"), QString("<font face=""Calibri"" size=""3"" color=""gray"">Nie można otworzyć pliku konfiguracji bazy danych.</font>"));
        msgBox.setStyleSheet("QMessageBox {background: white;}"
                             "QPushButton:hover {"
                             "border-radius: 5px;"
                             "background: rgb(255,140,0);"
                             "}"
                             "QPushButton{"
                             "color: white;"
                             "border-radius: 5px;"
                             "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "stop: 0 rgba(255,140,0), stop: 0.7 rgb(255,105,0));"
                             "min-width: 70px;"
                             "min-height: 30px;"
                             "font-family: Calibri;"
                             "font-size: 12;"
                             "font-weight: bold;"
                             "}"
                             "QPushButton:pressed {"
                             "color: white;"
                             "border-radius: 5px;"
                             "background: rgb(255,105,0);"
                             "}"
                             );

        msgBox.setWindowIcon(QIcon(":/images/images/icon.ico"));
        msgBox.exec();
        return 0;
    }
    QTextStream in(&initFile);
    QString line = in.readLine();
    initFile.close();

    if(!line.isEmpty()){
        const QStringList parameters = line.split(";");
        Database::setParameters(parameters.at(0),
                                parameters.at(1).toInt(),
                                parameters.at(2),
                                parameters.at(3),
                                parameters.at(4));

        // check connection
        if(Database::connectToDatabase()) { // connection ok
            Database::purgeDatabase();
            Database::setParameters(parameters.at(0),
                                    parameters.at(1).toInt(),
                                    parameters.at(2),
                                    parameters.at(3),
                                    parameters.at(4));
        }
        else line.clear(); // connection error - run creator
    }

    if(line.isEmpty()){
        DBConfigDialog dbConfig(true);
        if(dbConfig.exec()==DBConfigDialog::Rejected)
            return 0;
    }


    MainWindow w;
    w.show();

    return a.exec();
}
