#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login = "root";
    password = "embedded"; //change password here

    if (connectToDatabase(login, password)) {
        ui->statusBar->showMessage("Połączono z użytkownikiem: " + login);
        carTable = new QSqlQueryModel(this);
        carTable->setQuery("SELECT * FROM car;");
        orderTable = new QSqlQueryModel(this);
        orderTable->setQuery("SELECT * FROM reservation;");

        for(int i = 0; i < carTable->rowCount(); ++i) {
            carBlockVector.emplace_back(std::move(new CarBlock(carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                               carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,4)).toDate(),
                                                               carTable->data(carTable->index(i,5)).toDate(), carTable->data(carTable->index(i,6)).toInt(),
                                                               carTable->data(carTable->index(i,7)).toString(), static_cast<CarBlock::Status>(carTable->data(carTable->index(i,8)).toInt()),
                                                               carTable->data(carTable->index(i,9)).toString()
                                                               )

                                                  ));
        }

        scrollWidget = new QWidget(ui->scrollArea);
        scrollLayout = new QVBoxLayout(scrollWidget);
        for(auto pos= carBlockVector.begin();pos!=carBlockVector.end();++pos)
            scrollLayout->addWidget(*pos);
        ui->scrollArea->setWidget(scrollWidget);
    }
    else ui->statusBar->showMessage("Nie można połączyć z bazą danych");
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::connectToDatabase(QString &login, QString &password)
{
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    sqlDatabase.setHostName("127.0.0.1"); //server 192.168.1.7
    sqlDatabase.setDatabaseName("sigmacars");
    if(login.isEmpty() && password.isEmpty()) {
        sqlDatabase.setUserName("root");
        sqlDatabase.setPassword("Serwis4q@");
    }
    else {
        sqlDatabase.setUserName(login);
        sqlDatabase.setPassword(password);
    }

    if (!sqlDatabase.open()) return false;
    else return true;
}

void MainWindow::closeDatabase(){
    sqlDatabase.close();
    QSqlDatabase::removeDatabase("sigmacars");
}
