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


    notesTable = new QSqlQueryModel(this);
    notesTable->setQuery("SELECT * FROM notes WHERE isRead = 0 ORDER BY Datetime DESC LIMIT 10;");
    carTable = new QSqlQueryModel(this);
    carTable->setQuery("SELECT * FROM car;");
    bookingTable = new QSqlQueryModel(this);
    bookingTable->setQuery("SELECT * FROM booking;");
    carBlockVector.clear();
    notesActionsVector.clear();

    loadTrayIcon();


    CarBlock * lastCarBlock{nullptr};
    for(int i = 0; i < carTable->rowCount(); ++i) {

        carBlockVector.emplace_back(std::move(new CarBlock(carTable->data(carTable->index(i,0)).toInt(),
                                                           carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                           carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,4)).toDate(),
                                                           carTable->data(carTable->index(i,5)).toDate(), carTable->data(carTable->index(i,6)).toInt(),
                                                           static_cast<CarBlock::Status>(carTable->data(carTable->index(i,7)).toInt()), carTable->data(carTable->index(i,8)).toString()
                                                          )
                                              ));
       lastCarBlock = carBlockVector.back();
       lastCarBlock->setBookingTable(bookingTable);
       lastCarBlock->setAdminPermissions(true);
       connect(this, SIGNAL(trayMenuNoteClicked(int, int)), lastCarBlock, SLOT(showNotesDialog(int, int)));

       connect(carBlockVector[i],SIGNAL(carDeleted()),this,SLOT(updateView()));


    }
    carBlockVector.emplace_back(std::move(new CarBlock(0,QString("-----"),QString("-----"),QString("------"),
                                                       QDate::currentDate(),QDate::currentDate(),
                                                       0,CarBlock::Rented,QString(":/images/images/car.png"),true)));
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

// ----- TRAY ICON METHODS--------------

void MainWindow::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    restoreAction->setEnabled(!visible);
    QMainWindow::setVisible(visible);

    if(visible) {
        dcAction = minimizeAction;
        minimizeAction->setFont(QFont("Calibri", 9, QFont::Bold));
        restoreAction->setFont(QFont("Calibri", 9));
    }
    else {
        dcAction = restoreAction;
        restoreAction->setFont(QFont("Calibri", 9, QFont::Bold));
        minimizeAction->setFont(QFont("Calibri", 9));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {

        QMessageBox msgBox(QMessageBox::Question, tr("Rezerwacja samochodów"), tr("<font face=""Calibri Light"" size=""5"" color=""gray""><b>Czy chcesz zminimalizować program do paska zadań?</b></font>"), QMessageBox::Yes | QMessageBox::No );

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
                             "width: 100;"
                             "height: 40;"
                             "font-family: Calibri Light;"
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
        msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
        msgBox.setButtonText(QMessageBox::No, tr("Nie"));
        if (msgBox.exec() == QMessageBox::No) {
            QApplication::quit();
        }
        hide();
        event->ignore();
    }
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->setStyleSheet("QMenu {"
                                "background-color: orange;"
                                //"background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                //"stop: 0 rgba(255,140,0), stop: 0.7 rgb(255,105,0));"
                                "}"

                                "QMenu::icon {"
                                "border: false;"
                                "}"

                                "QMenu::separator {"
                                "height: 1px;"
                                "background: gray;"
                                "margin-left: 2px;"
                                "margin-right: 2px;"
                                "}"
                                );
    notesMenu = new QMenu(this);
    notesMenu->setTitle(QString("Wiadomości (%1)").arg(newMessagesNumber));
    notesMenu->setStyleSheet("QMenu {"
                             "background-color: orange;"
                             //"background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             //"stop: 0 rgba(255,140,0), stop: 0.7 rgb(255,105,0));"
                             "}"

                            );

    if(newMessagesNumber) {
        notesMenu->setIcon(QIcon(":/images/images/new.png"));
    }
    else {
        notesMenu->setIcon(QIcon(":/images/images/read.png"));
    }

    for(auto i : notesActionsVector)
        notesMenu->addAction(i);

    trayIconMenu->addMenu(notesMenu);
    trayIconMenu->addSeparator();
    minimizeAction->setIcon(QIcon(":/images/images/minimize.png"));
    trayIconMenu->addAction(minimizeAction);
    restoreAction->setIcon(QIcon(":/images/images/restore.png"));
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    quitAction->setIcon(QIcon(":/images/images/close.png"));
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(notesMenu, SIGNAL(triggered(QAction*)), this, SLOT(noteActionClicked(QAction*)));

}

void MainWindow::loadTrayIcon()
{
    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    setIcon();

}

void MainWindow::setIcon()
{
    trayIcon->setIcon(QIcon(":/images/images/icon.ico"));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
            break;
    case QSystemTrayIcon::DoubleClick:
            dcAction->activate(QAction::Trigger);
            break;
    case QSystemTrayIcon::MiddleClick:
            break;
    default:
        ;
    }
}

void MainWindow::noteActionClicked(QAction * act)
{
    emit trayMenuNoteClicked(act->data().toString().split(",").at(0).toInt(), act->data().toString().split(",").at(1).toInt()); // pass idCar to open NotesDialog (slot showNotesDialog)
}

void MainWindow::createActions()
{

    QAction * lastAction{nullptr};
    QString actionData;

    newMessagesNumber = 0;


    for(int i = 0; i < notesTable->rowCount(); ++i) {


        notesActionsVector.emplace_back(std::move( new QAction(QString("&%1 %2").arg(notesTable->data(notesTable->index(i,2)).toString()).arg(notesTable->data(notesTable->index(i,3)).toString()), this) ));
        lastAction = notesActionsVector.back();

       lastAction->setFont(QFont("Calibri", 9, QFont::Bold));
       ++newMessagesNumber;
       actionData = notesTable->data(notesTable->index(i,0)).toString() + QString(",") + notesTable->data(notesTable->index(i,6)).toString();
       lastAction->setData(actionData);


    }


    minimizeAction = new QAction(QString("Minimalizuj"), this);
    minimizeAction->setFont(QFont("Calibri", 9));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(QString("Przywróć"), this);
    restoreAction->setFont(QFont("Calibri", 9));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));

    quitAction = new QAction(QString("Zamknij"), this);
    quitAction->setFont(QFont("Calibri", 9));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}


void MainWindow::showTrayIcon()
{
   trayIcon->show();
}

void MainWindow::showMessage()
{
    QString title = "Wiadomość";
    QString msg = "Treść przykładowej wiadomości";
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    trayIcon->showMessage(title, msg, icon, 5000);
}

void MainWindow::setPopupMessage()
{

}
