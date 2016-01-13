#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login = "root";
    password = "Serwis4q@";



    if (connectToDatabase(login, password)) ui->statusBar->showMessage("Połączono z użytkownikiem: " + login);
    else ui->statusBar->showMessage("Połączono z użytkownikiem: " + login);

    carTable = new QSqlQueryModel(this);
    carTable->setQuery("SELECT * FROM car;");

    orderTable = new QSqlQueryModel(this);
    orderTable->setQuery("SELECT * FROM order;");

    for(int i = 0; i < model->rowCount(); ++i) {
        carBlockVector.emplace_back(std::move(new CarBlock(carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                           carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,4)).toString(),
                                                           carTable->data(carTable->index(i,5)).toDate(), carTable->data(carTable->index(i,6)).toDate(),
                                                           carTable->data(carTable->index(i,7)).toInt(), carTable->data(carTable->index(i,8)).toString()
                                                           )

                                              ));
      // to do
    }

    scrollLayout = new QVBoxLayout(ui->scrollArea);
    for(auto pos=carBlockVector.begin();pos!=carBlockVector.end();++pos)
        scrollLayout->addWidget(*pos);

    scrollWidget = new QWidget(ui->centralWidget);
    scrollWidget->setLayout(scrollLayout);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(scrollWidget);

}

MainWindow::~MainWindow()
{

    delete ui;
}

bool MainWindow::connectToDatabase(QString &login, QString &password)
{
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    sqlDatabase.setHostName("192.168.1.7");
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
