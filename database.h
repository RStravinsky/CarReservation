#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

class Database : public QObject
{
    Q_OBJECT

private:
    static QSqlDatabase sqlDatabase;

public:
    explicit Database(QObject *parent = 0);
    static QSqlDatabase getDatabase() {return sqlDatabase;}
    static bool connectToDatabase(QString login, QString password);
    static void closeDatabase();

signals:

public slots:

};

#endif // DATABASE_H
