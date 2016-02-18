#include "mainwindow.h"
#include "ui_mainwindow.h"

#define UPDATE_TIME 20000
#define ADMIN_PASSWD "Admin4q@"

std::shared_ptr<NotesDialog> notesDialogPointer;
bool isAdmin{false};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login = "rezerwacja";
    password = "rezerwacja"; //change password here

    database = new Database();

    createHelpButton();
    createUpdateButton();
    createBackupButton();
    ui->statusBar->setStyleSheet("background: white; color: gray; font-family: Calibri; font-size: 10pt;");

    createLoginOption();
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOverflow()));
    onTimerOverflow();
    loadTrayIcon();


//    QMovie *movie = new QMovie("//k1/DBIR/Programowanie/Aplikacja REZERWACJA/lblHoliday/holiday.gif");
//    ui->lblHoliday->setMovie(movie);
//    movie->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimerOverflow()
{
    updateView(true);
    timer->start(UPDATE_TIME);
}

void MainWindow::createBackup()
{
    QString currentTime=QTime::currentTime().toString();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Ścieżka zapisu"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if(fileName.isEmpty())
        return;

    QString backUpPath= fileName +"/SigmaCars_"+QDate::currentDate().toString().replace(" ","_")+"_"+currentTime.replace(":","_")+".sql";
    backUpPath.replace("file:","");

    QString Cmd = QString("mysqldump.exe -u%1 -h%2 -p%3 --routines sigmacars").arg("rezerwacja","192.168.1.7","rezerwacja");
    QString Path = QString("%1").arg(backUpPath);
    QProcess poc;
    poc.setStandardOutputFile(Path);
    poc.start(Cmd);
    poc.waitForFinished( 30000 );

    QMessageBox::information(this,"Informacja", "Kopia zapasowa została zapisana w folderze \n"
                                                +backUpPath);
}


void MainWindow::updateView(bool isCopyEnable)
{  
    //qDebug() << "Updating..." << endl;
    if(Database::connectToDatabase("rezerwacja","rezerwacja")) {

        ui->statusBar->showMessage("Połączono z bazą danych");
        const int varticalPosition = ui->scrollArea->verticalScrollBar()->value();

        if(isAdmin)
            copyEnable = isCopyEnable;

        delete carTable;
        carTable = new QSqlQueryModel(this);
        carTable->setQuery("SELECT * FROM car;");

        delete bookingTable;
        bookingTable = new QSqlQueryModel(this);
        bookingTable->setQuery("SELECT * FROM booking;");

        if(notesTable != nullptr)
            lastRowCount = notesTable->rowCount();

        // Copy last CarBlock
        CarBlock * element;
        if (isAdmin && carBlockVector.size()>0)
                element = new CarBlock(*(carBlockVector.at(carBlockVector.size()-1)));

        carBlockVector.clear();
        CarBlock * lastCarBlock{nullptr};
        for(int i = 0; i < carTable->rowCount(); ++i) {
            if(carTable->data(carTable->index(i,9)).toBool()) {
                carBlockVector.emplace_back(std::move(new CarBlock(false, carTable->data(carTable->index(i,0)).toInt(),
                                                                   carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                                   carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,4)).toDate(),
                                                                   carTable->data(carTable->index(i,5)).toDate(), carTable->data(carTable->index(i,6)).toInt(),
                                                                   static_cast<CarBlock::Status>(carTable->data(carTable->index(i,7)).toInt()), carTable->data(carTable->index(i,8)).toString(),
                                                                   carTable->data(carTable->index(i,9)).toBool()
                                                                  )
                                                      ));
               lastCarBlock = carBlockVector.back();
               lastCarBlock->setBookingTable(bookingTable);
               lastCarBlock->setAdminPermissions(isAdmin);
               connect(this, SIGNAL(trayMenuNoteClicked(int, int)), lastCarBlock, SLOT(showNotesDialog(int, int)), Qt::DirectConnection);
               connect(carBlockVector.back(),SIGNAL(changeStatusBar(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
               connect(carBlockVector.back(),SIGNAL(carDeleted(bool)),this,SLOT(updateView(bool)),Qt::QueuedConnection);
               connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()), Qt::QueuedConnection);
               connect(carBlockVector.back(),&CarBlock::progressFinished, this, [=](){timer->stop();timer->start(UPDATE_TIME);});
               connect(carBlockVector.back(),&CarBlock::noteClosed, this,[=](){loadTrayIcon();}, Qt::QueuedConnection);
           }
        }
        if(isAdmin) {
            setPopupMessage();

            for(int i = 0; i < carTable->rowCount(); ++i) {
                if(!carTable->data(carTable->index(i,9)).toBool()) {
                    carBlockVector.emplace_back(std::move(new CarBlock(false, carTable->data(carTable->index(i,0)).toInt(),
                                                                       carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                                       carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,4)).toDate(),
                                                                       carTable->data(carTable->index(i,5)).toDate(), carTable->data(carTable->index(i,6)).toInt(),
                                                                       static_cast<CarBlock::Status>(carTable->data(carTable->index(i,7)).toInt()), carTable->data(carTable->index(i,8)).toString(),
                                                                       carTable->data(carTable->index(i,9)).toBool()
                                                                      )
                                                          ));
                   lastCarBlock = carBlockVector.back();
                   lastCarBlock->setBookingTable(bookingTable);
                   lastCarBlock->setAdminPermissions(isAdmin);
                   connect(this, SIGNAL(trayMenuNoteClicked(int, int)), lastCarBlock, SLOT(showNotesDialog(int, int)), Qt::DirectConnection);
                   connect(carBlockVector.back(),SIGNAL(changeStatusBar(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
                   connect(carBlockVector.back(),SIGNAL(carDeleted(bool)),this,SLOT(updateView(bool)),Qt::QueuedConnection);
                   connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()), Qt::QueuedConnection);
                   connect(carBlockVector.back(),&CarBlock::progressFinished, this, [=](){timer->start(UPDATE_TIME);});
                   connect(carBlockVector.back(),&CarBlock::noteClosed, this,[=](){loadTrayIcon();}, Qt::QueuedConnection);
               }
            }

            if(!copyEnable)
                carBlockVector.emplace_back(std::move(new CarBlock()));

            else carBlockVector.push_back(std::move(element));

            connect(carBlockVector.back(),SIGNAL(carAdded(bool)),this,SLOT(updateView(bool)),Qt::QueuedConnection);
            connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()));
            connect(carBlockVector.back(),&CarBlock::progressFinished,[=](){timer->start(UPDATE_TIME);});
            copyEnable = true;
        }

        delete scrollLayout;
        delete scrollWidget;
        scrollWidget = new QWidget(ui->scrollArea);
        scrollLayout = new QVBoxLayout(scrollWidget);
        for(auto pos= carBlockVector.begin();pos!=carBlockVector.end();++pos)
            scrollLayout->addWidget(*pos);
        ui->scrollArea->setWidget(scrollWidget);
        ui->scrollArea->verticalScrollBar()->setValue(varticalPosition);

        Database::closeDatabase();
    }

    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        ui->statusBar->showMessage("Nie można połączyć z bazą danych");
    }
}

void MainWindow::reloadNotes()
{
    if(notesTable != nullptr) {
        delete notesTable;
    }

    notesTable = new QSqlQueryModel(this);
    notesTable->setQuery("SELECT * FROM notes WHERE isRead = 0 ORDER BY Datetime DESC, idNotes DESC;");
    notesActionsVector.clear();
}

void MainWindow::createUpdateButton()
{
    QPushButton * updateButton = new QPushButton(this);
    updateButton->setIcon(QIcon(":/images/images/update.png"));
    updateButton->setToolTip("Aktualizuj");
    updateButton->setStyleSheet("border:none;");
    ui->statusBar->addPermanentWidget(updateButton);
    connect(updateButton, &QPushButton::clicked,[=](){updateView(true);});
}

void MainWindow::createHelpButton()
{
    QPushButton * helpButton = new QPushButton(this);
    helpButton->setIcon(QIcon(":/images/images/help.png"));
    helpButton->setText("Jak wypożyczyć ");
    helpButton->setStyleSheet("border:none; color: gray");
    ui->statusBar->addPermanentWidget(helpButton);
    connect(helpButton, &QPushButton::clicked,[=](){HelpDialog h; h.exec();});
}

void MainWindow::createBackupButton()
{
    backupButton = new QPushButton(this);
    backupButton->setIcon(QIcon(":/images/images/backup.png"));
    backupButton->setToolTip("Kopia bezpieczeństwa");
    backupButton->setStyleSheet("border:none; color: gray");
    ui->statusBar->addPermanentWidget(backupButton);
    connect(backupButton, &QPushButton::clicked,[=](){createBackup();});

    setBackupButtonVisible();
}

void MainWindow::setBackupButtonVisible()
{
    if(!isAdmin)
        backupButton->setVisible(false);
    if(isAdmin)
        backupButton->setVisible(true);
}

void MainWindow::createLoginOption()
{
    QPushButton * loginButton = new QPushButton(this);
    loginButton->setIcon(QIcon(":/images/images/key.png"));
    loginButton->setStyleSheet("border:none;");
    loginButton->setToolTip("Zaloguj jako administrator");

    QPushButton * logoutButton = new QPushButton(this);
    logoutButton->setIcon(QIcon(":/images/images/exit.png"));
    logoutButton->setStyleSheet("border:none;");
    logoutButton->setToolTip("Wyloguj");
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
        if(Database::getDatabase().isOpen()) Database::closeDatabase();
        if(Database::connectToDatabase("rezerwacja","rezerwacja")) {
            if(adminPassword->text()==ADMIN_PASSWD) {
                    isAdmin = true;
                    setBackupButtonVisible();
                    loginButton->click();
                    loginButton->setVisible(false);
                    logoutButton->setVisible(true);
                    Database::closeDatabase();
                    updateView(false);
                    loadTrayIcon();
            }
        }
        else {
            Database::closeDatabase();
            QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
            ui->statusBar->showMessage("Nie można połączyć z bazą danych");
        }
    });
    connect(logoutButton, &QPushButton::clicked, [=]() {
        if(Database::getDatabase().isOpen()) Database::closeDatabase();
        if(Database::connectToDatabase("rezerwacja","rezerwacja")) {
            isAdmin = false;
            setBackupButtonVisible();
            loginButton->setVisible(true);
            logoutButton->setVisible(false);
            updateView(true);
            loadTrayIcon();
            Database::closeDatabase();
        }
        else {
            Database::closeDatabase();
            QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
            ui->statusBar->showMessage("Nie można połączyć z bazą danych");
        }
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

void MainWindow::createActions(bool _isAdmin)
{
    if(_isAdmin) {
        QString actionData;
        newMessagesNumber = 0;
        for(int i = 0; i < notesTable->rowCount(); ++i) {
           notesActionsVector.emplace_back(std::move( new QAction(QString("&%1 %2").arg(notesTable->data(notesTable->index(i,2)).toString()).arg(notesTable->data(notesTable->index(i,3)).toString()), this) ));
           notesActionsVector.back()->setFont(QFont("Calibri", 9, QFont::Bold));

           ++newMessagesNumber;
           actionData = notesTable->data(notesTable->index(i,0)).toString() + QString(",") + notesTable->data(notesTable->index(i,6)).toString();
           notesActionsVector.back()->setData(actionData);
        }
    }

    delete minimizeAction;
    minimizeAction = new QAction(QString("Minimalizuj"), this);
    minimizeAction->setFont(QFont("Calibri", 9));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    delete restoreAction;
    restoreAction = new QAction(QString("Przywróć"), this);
    restoreAction->setFont(QFont("Calibri", 9));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));

    delete quitAction;
    quitAction = new QAction(QString("Zamknij"), this);
    quitAction->setFont(QFont("Calibri", 9));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon(bool _isAdmin)
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
  if(_isAdmin) {

        delete notesMenu;
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

        connect(notesMenu, SIGNAL(triggered(QAction*)), this, SLOT(noteActionClicked(QAction*)), Qt::QueuedConnection );
  }

  minimizeAction->setIcon(QIcon(":/images/images/minimize.png"));
  trayIconMenu->addAction(minimizeAction);
  restoreAction->setIcon(QIcon(":/images/images/restore.png"));
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  quitAction->setIcon(QIcon(":/images/images/close.png"));
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::loadTrayIcon()
{
    if(Database::getDatabase().isOpen())
        Database::closeDatabase();

    if(Database::connectToDatabase("rezerwacja","rezerwacja")) {
        reloadNotes();

        delete trayIcon;
        delete trayIconMenu;

        createActions(isAdmin);
        createTrayIcon(isAdmin);
        setIcon();
        showTrayIcon();
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
        connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(poupMessageClicked()));
        Database::closeDatabase();
    }
    else {
        Database::closeDatabase();
        ui->statusBar->showMessage("Nie można połączyć z bazą danych");
    }
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

void MainWindow::poupMessageClicked()
{
    if(Database::getDatabase().isOpen())
        Database::closeDatabase();

    if(Database::connectToDatabase("rezerwacja","rezerwacja")) {
        notesTable->setQuery("SELECT * FROM notes WHERE isRead = 0 ORDER BY Datetime DESC, idNotes DESC;");
        emit trayMenuNoteClicked(notesTable->data(notesTable->index(0,0)).toInt(), notesTable->data(notesTable->index(0,6)).toInt());
        Database::closeDatabase();
    }
}

void MainWindow::setPopupMessage()
{
    reloadNotes();
    int actualRowCount = notesTable->rowCount();
    if( actualRowCount > lastRowCount) {
        QString newNote = notesTable->index(0,2).data().toString() + QString(" ") + notesTable->index(0,3).data().toString();
        QString title = "Nowa uwaga:";
        loadTrayIcon();
        trayIcon->showMessage(title, newNote, QSystemTrayIcon::Information, 10000);
    }
}

