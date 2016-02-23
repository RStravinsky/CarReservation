#include "database.h"

QSqlDatabase Database::sqlDatabase = QSqlDatabase();

Database::Database(QObject *parent) : QObject(parent)
{
}

bool Database::connectToDatabase(QString login, QString password)
{
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    sqlDatabase.setHostName("192.168.1.7");
    sqlDatabase.setDatabaseName("sigmacars");
    sqlDatabase.setUserName(login);
    sqlDatabase.setPassword(password);
    if (!sqlDatabase.open()) return false;
    else return true;
}

void Database::closeDatabase()
{
    QString connection;
    connection = sqlDatabase.connectionName();
    sqlDatabase.close();
    sqlDatabase = QSqlDatabase();
    sqlDatabase.removeDatabase(connection);
}
