#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login = "root";
    password = "Serwis4q@"; //change password here


    if (connectToDatabase(login, password)) {
        ui->statusBar->showMessage("Połączono z użytkownikiem: " + login);
        updateView();
    }
    else ui->statusBar->showMessage("Nie można połączyć z bazą danych");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateView()
{
    delete carTable;
    delete bookingTable;
    delete scrollLayout;
    delete scrollWidget;

    carTable = new QSqlQueryModel(this);
    carTable->setQuery("SELECT * FROM car;");
    bookingTable = new QSqlQueryModel(this);
    bookingTable->setQuery("SELECT * FROM booking;");
    carBlockVector.clear();

    CarBlock * lastCarBlock{nullptr};
    for(int i = 0; i < carTable->rowCount(); ++i) {
        carBlockVector.emplace_back(std::move(new CarBlock(carTable->data(carTable->index(i,0)).toInt(),
                                                           carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                           carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,4)).toDate(),
                                                           carTable->data(carTable->index(i,5)).toDate(), carTable->data(carTable->index(i,6)).toInt(),
                                                           carTable->data(carTable->index(i,7)).toString(), static_cast<CarBlock::Status>(carTable->data(carTable->index(i,8)).toInt()),
                                                           carTable->data(carTable->index(i,9)).toString()
                                                          )
                                              ));
       lastCarBlock = carBlockVector.back();
       lastCarBlock->setBookingTable(bookingTable);
       lastCarBlock->setAdminPermissions(true);
       connect(carBlockVector[i],SIGNAL(carDeleted()),this,SLOT(updateView()));

    }
    carBlockVector.emplace_back(std::move(new CarBlock(0,QString("-----"),QString("-----"),QString("------"),
                                                       QDate::currentDate(),QDate::currentDate(),
                                                       0,QString("-----"),CarBlock::Rented,QString(":/images/images/car.png"),true)));
    connect(carBlockVector.back(),SIGNAL(carAdded()),this,SLOT(updateView()));


    scrollWidget = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);
    for(auto pos= carBlockVector.begin();pos!=carBlockVector.end();++pos)
        scrollLayout->addWidget(*pos);
    ui->scrollArea->setWidget(scrollWidget);
}

bool MainWindow::connectToDatabase(QString &login, QString &password)
{
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    sqlDatabase.setHostName("192.168.1.7"); //server 192.168.1.7
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

void MainWindow::closeDatabase()
{
    sqlDatabase.close();
    QSqlDatabase::removeDatabase("sigmacars");
}
