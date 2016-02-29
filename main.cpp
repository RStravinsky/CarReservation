#include "mainwindow.h"
#include "dbconfigdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile initFile( QDir::currentPath()+"/init.txt" );
    if (!initFile.open(QIODevice::ReadOnly))
            return 0;

    QTextStream in(&initFile);
    const QString line = in.readLine();
    initFile.close();

    if(line.isEmpty()) {
        DBConfigDialog dbConfig(true);
        if(dbConfig.exec()==DBConfigDialog::Rejected)
            return 0;
    }
    else {
        const QStringList parameters = line.split(";");
        Database::setParameters(parameters.at(0),
                                parameters.at(1).toInt(),
                                parameters.at(2),
                                parameters.at(3),
                                parameters.at(4));
    }

    MainWindow w;
    w.show();

    return a.exec();
}
