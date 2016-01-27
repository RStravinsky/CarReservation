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
    }
    else ui->statusBar->showMessage("Nie można połączyć z bazą danych");

    createLoginOption();
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOverflow()));
    onTimerOverflow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimerOverflow()
{
    qDebug() << "Update mainwindow" << endl;
    const int varticalPosition = ui->scrollArea->verticalScrollBar()->value();
    updateView(true);
    ui->scrollArea->verticalScrollBar()->setValue(varticalPosition);
    timer->start(2000);
}

void MainWindow::updateView(bool isCopyEnable)
{  
    if(isAdmin){
        if(isCopyEnable) copyEnable = true;
        else copyEnable = false;
    }

    delete carTable;  
    carTable = new QSqlQueryModel(this);
    carTable->setQuery("SELECT * FROM car;");

    delete bookingTable;
    bookingTable = new QSqlQueryModel(this);
    bookingTable->setQuery("SELECT * FROM booking;");

    notesTable = new QSqlQueryModel(this);
    notesTable->setQuery("SELECT * FROM notes WHERE isRead = 0 ORDER BY Datetime DESC;");
    notesActionsVector.clear();

    loadTrayIcon();

    // Copy last CarBlock
    CarBlock * element;
    if (isAdmin)
            element = new CarBlock(*(carBlockVector.at(carBlockVector.size()-1)));

    carBlockVector.clear();
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
       lastCarBlock->setAdminPermissions(isAdmin);
       connect(this, SIGNAL(trayMenuNoteClicked(int, int)), lastCarBlock, SLOT(showNotesDialog(int, int)));
       connect(carBlockVector.back(),SIGNAL(carDeleted(bool)),this,SLOT(updateView(bool)));
       connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()));
       connect(carBlockVector.back(),&CarBlock::progressFinished,[=](){timer->start(5000);});
       connect(carBlockVector.back(),&CarBlock::noteClosed,[=](){delete trayIconMenu; delete trayIcon; updateView(true); showTrayIcon();});
    }

    if(isAdmin) {
        if(!copyEnable)
            carBlockVector.emplace_back(std::move(new CarBlock(0,QString("-----"),QString("-----"),QString("------"),
                                                               QDate::currentDate(),QDate::currentDate(),
                                                               0,CarBlock::Rented,QString(":/images/images/car.png"),true)));

        else carBlockVector.push_back(std::move(element));

        connect(carBlockVector.back(),SIGNAL(carAdded(bool)),this,SLOT(updateView(bool)));
        connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()));
        connect(carBlockVector.back(),&CarBlock::progressFinished,[=](){timer->start(5000);});
        copyEnable = true;
    }

    delete scrollLayout;
    delete scrollWidget;
    scrollWidget = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);
    for(auto pos= carBlockVector.begin();pos!=carBlockVector.end();++pos) {
        //qDebug() << *pos << endl;
        scrollLayout->addWidget(*pos);
    }
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

void MainWindow::createLoginOption()
{
    QPushButton * loginButton = new QPushButton(this);
    loginButton->setIcon(QIcon(":/images/images/key.png"));
    loginButton->setStyleSheet("border:none;");

    QPushButton * logoutButton = new QPushButton(this);
    logoutButton->setIcon(QIcon(":/images/images/exit.png"));
    logoutButton->setStyleSheet("border:none;");
    logoutButton->setVisible(false);

    QLineEdit *  adminPassword = new QLineEdit(this);
    adminPassword->setVisible(false);
    adminPassword->setEchoMode(QLineEdit::Password);
    adminPassword->setFixedWidth(100);

    ui->statusBar->addPermanentWidget(loginButton);
    ui->statusBar->addPermanentWidget(logoutButton);
    ui->statusBar->addPermanentWidget(adminPassword);

    connect(loginButton, &QPushButton::clicked, [=]() {
        static bool visible = false;
        visible = !visible;
        adminPassword->clear();
        adminPassword->setFocus();
        adminPassword->setVisible(visible);
    });

    connect(adminPassword, &QLineEdit::editingFinished, [=]() {
        if(adminPassword->text()=="sigma") {
            isAdmin = true;
            loginButton->click();
            loginButton->setVisible(false);
            logoutButton->setVisible(true);
            updateView(false);
        }
    });

    connect(logoutButton, &QPushButton::clicked, [=]() {
        isAdmin = false;
        loginButton->setVisible(true);
        logoutButton->setVisible(false);
        updateView(true);
    });
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

void MainWindow::showTrayIcon()
{
    trayIcon->show();
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
                                "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                "stop: 0 rgba(255,140,0), stop: 0.7 rgb(255,105,0));"
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
                             "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "stop: 0 rgba(255,140,0), stop: 0.7 rgb(255,105,0));"
                             "}"
                            );

    if(newMessagesNumber) {
        notesMenu->setIcon(QIcon(":/images/images/new.png"));
    }
    else {
        notesMenu->setIcon(QIcon(":/images/images/read.png"));
    }


    if(notesActionsVector.size()<=10) {
        for(auto elem: notesActionsVector)
            notesMenu->addAction(elem);
    }
    else
        for(int i=0; i<10; ++i)
            notesMenu->addAction(notesActionsVector.at(i));

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

void MainWindow::setPopupMessage()
{

}
