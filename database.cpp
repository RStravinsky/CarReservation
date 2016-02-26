#include "database.h"

QSqlDatabase Database::sqlDatabase = QSqlDatabase();

Database::Database(QObject *parent) : QObject(parent)
{
}

bool Database::connectToDatabase()
{
    if (!sqlDatabase.open()) return false;
    else return true;
}

void Database::setParameters(const QString & hostname, int port, const QString & database, const QString & user, const QString & password)
{
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    sqlDatabase.setHostName(hostname);
    sqlDatabase.setPort(port);
    sqlDatabase.setDatabaseName(database);
    sqlDatabase.setUserName(user);
    sqlDatabase.setPassword(password);
}

void Database::purgeDatabase()
{
    QString connection;
    connection = sqlDatabase.connectionName();
    sqlDatabase.close();
    sqlDatabase = QSqlDatabase();
    qDebug() << "+++" <<QSqlDatabase::connectionNames() << "+++";
    sqlDatabase.removeDatabase(connection);
}
