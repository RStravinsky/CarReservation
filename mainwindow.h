#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QtSql>
#include <QSqlQueryModel>
#include <vector>

#include "carblock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QVBoxLayout *scrollLayout;
    QWidget *scrollWidget;
    QSqlDatabase sqlDatabase;
    QSqlQueryModel * carTable;
    QSqlQueryModel * bookingTable;
    std::vector<CarBlock*> carBlockVector;
    QString login;
    QString password;

    bool connectToDatabase(QString &login, QString &password);
    void closeDatabase();
};

#endif // MAINWINDOW_H
