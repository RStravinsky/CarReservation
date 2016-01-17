#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QtSql/QSqlQueryModel>
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

public slots:
    void updateView();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *scrollLayout{nullptr};
    QWidget *scrollWidget{nullptr};
    QSqlDatabase sqlDatabase;
    QSqlQueryModel * carTable{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    std::vector<CarBlock*> carBlockVector;
    QString login;
    QString password;

    bool connectToDatabase(QString &login, QString &password);
    void closeDatabase();
};

#endif // MAINWINDOW_H
