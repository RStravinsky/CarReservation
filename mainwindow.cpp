#include "mainwindow.h"
#include "ui_mainwindow.h"

#define UPDATE_TIME 5000
#define ADMIN_PASSWD "Admin4q@"

std::shared_ptr<NotesDialog> notesDialogPointer;
bool isAdmin{false};

bool MainWindow::isDatabase = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->setStyleSheet("QStatusBar{background: white; color: gray; font-family: Calibri; font-size: 10pt;}"
                                 "QLabel{border:none; color:gray; font-family: Calibri; font-size: 10pt;}"
                                 "QStatusBar::item{border: none;}");
    statusLabel = new QLabel(this);
    ui->statusBar->addWidget(statusLabel);

    createHelpButton();
    createUpdateButton();
    createChangePSW();
    createBackupButton();
    createDBConfigButton();
    createLoginOption();

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOverflow()));
    onTimerOverflow();
    loadTrayIcon();
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

    QString backUpPath= fileName +"/BACKUP_"+QDate::currentDate().toString().replace(" ","_")+"_"+currentTime.replace(":","_")+".sql";
    backUpPath.replace("file:","");

    QString Cmd = QString("mysqldump.exe -u%1 -h%2 -p%3 --routines testsigmadb").arg(DBConfigDialog::user,DBConfigDialog::currentAddress,DBConfigDialog::password);
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
    qDebug() << endl;
    qDebug() << "Updating...";
    if(isDatabase){
        if(Database::isOpen()) {
            qDebug() << "is Open";
            //ui->statusBar->showMessage("Połączono z bazą danych: " + Database::returnHostname());
            statusLabel->setText("Połączono z bazą danych: " + Database::returnHostname());
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
                                                                       carTable->data(carTable->index(i,9)).toBool(),carTable->data(carTable->index(i,10)).toInt()
                                                                      )
                                                          ));
                   lastCarBlock = carBlockVector.back();
                   lastCarBlock->setBookingTable(bookingTable);
                   lastCarBlock->setAdminPermissions(isAdmin);
                   connect(this, SIGNAL(trayMenuNoteClicked(int, int)), lastCarBlock, SLOT(showNotesDialog(int, int)), Qt::DirectConnection);
                   connect(carBlockVector.back(),SIGNAL(changeStatusBar(QString)),statusLabel,SLOT(setText(QString)));
                   connect(carBlockVector.back(),SIGNAL(carDeleted(bool)),this,SLOT(updateView(bool)),Qt::QueuedConnection);
                   //connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()), Qt::QueuedConnection);
                   connect(carBlockVector.back(),&CarBlock::inProgress,this,[=](){ timer->stop(); this->setEnabled(false); qDebug() << "In progress" ;});
                   connect(carBlockVector.back(),&CarBlock::progressFinished, this, [=](){timer->start(UPDATE_TIME); this->setEnabled(true); qDebug() << "Progress finished" ; });
                   connect(carBlockVector.back(),&CarBlock::noteClosed, this,[=](){loadTrayIcon();this->setEnabled(true);}, Qt::QueuedConnection);
               }
               else if(!carTable->data(carTable->index(i,9)).toBool() && isAdmin){
                    carBlockVector.emplace_back(std::move(new CarBlock(false, carTable->data(carTable->index(i,0)).toInt(),
                                                                       carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                                       carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,4)).toDate(),
                                                                       carTable->data(carTable->index(i,5)).toDate(), carTable->data(carTable->index(i,6)).toInt(),
                                                                       static_cast<CarBlock::Status>(carTable->data(carTable->index(i,7)).toInt()), carTable->data(carTable->index(i,8)).toString(),
                                                                       carTable->data(carTable->index(i,9)).toBool(),carTable->data(carTable->index(i,10)).toInt()
                                                                      )
                                                          ));
                   lastCarBlock = carBlockVector.back();
                   lastCarBlock->setBookingTable(bookingTable);
                   lastCarBlock->setAdminPermissions(isAdmin);
                   connect(this, SIGNAL(trayMenuNoteClicked(int, int)), lastCarBlock, SLOT(showNotesDialog(int, int)), Qt::DirectConnection);
                   //connect(carBlockVector.back(),SIGNAL(changeStatusBar(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
                   connect(carBlockVector.back(),SIGNAL(changeStatusBar(QString)),statusLabel,SLOT(setText(QString)));
                   connect(carBlockVector.back(),SIGNAL(carDeleted(bool)),this,SLOT(updateView(bool)),Qt::QueuedConnection);
                   //connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()), Qt::QueuedConnection);
                   //connect(carBlockVector.back(),&CarBlock::progressFinished, this, [=](){timer->start(UPDATE_TIME);});
                   connect(carBlockVector.back(),&CarBlock::inProgress,this,[=](){ timer->stop(); this->setEnabled(false); qApp->processEvents(); qDebug() << "In progress" ;});
                   connect(carBlockVector.back(),&CarBlock::progressFinished, this, [=](){timer->start(UPDATE_TIME); this->setEnabled(true); qApp->processEvents(); qDebug() << "Progress finished" ; });
                   connect(carBlockVector.back(),&CarBlock::noteClosed, this,[=](){loadTrayIcon();this->setEnabled(true);}, Qt::QueuedConnection);
                }
            }
            if(isAdmin) {
                setPopupMessage();
                if(!copyEnable)
                    carBlockVector.emplace_back(std::move(new CarBlock()));

                else carBlockVector.push_back(std::move(element));

                connect(carBlockVector.back(),SIGNAL(carAdded(bool)),this,SLOT(updateView(bool)),Qt::QueuedConnection);
                //connect(carBlockVector.back(),SIGNAL(inProgress()),timer,SLOT(stop()));
                //connect(carBlockVector.back(),&CarBlock::progressFinished,[=](){timer->start(UPDATE_TIME);});
                connect(carBlockVector.back(),&CarBlock::inProgress,this,[=](){ timer->stop(); this->setEnabled(false); qDebug() << "In progress" ;});
                connect(carBlockVector.back(),&CarBlock::progressFinished, this, [=](){timer->start(UPDATE_TIME); this->setEnabled(true); qDebug() << "Progress finished" ;});
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
        }

        else {
            QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
            //ui->statusBar->showMessage("Nie można połączyć z bazą danych");
            statusLabel->setText("Nie można połączyć z bazą danych");
        }
    }
    else{
        if(scrollLayout != nullptr) {
            qDebug() << "layout: " << scrollLayout << endl;
            delete scrollLayout;
            scrollLayout = nullptr;
        }

        if(scrollWidget != nullptr) {
            qDebug() << "layout: " << scrollWidget << endl;
            delete scrollWidget;
            scrollWidget = nullptr;
        }
        carBlockVector.clear();
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

void MainWindow::createChangePSW()
{
    changePSW = new QPushButton(this);
    changePSW->setIcon(QIcon(":/images/images/psw.png"));
    changePSW->setToolTip("Zmień hasło");
    changePSW->setStyleSheet("border:none;");
    ui->statusBar->addPermanentWidget(changePSW);
    connect(changePSW, &QPushButton::clicked,[=](){
        if(isDatabase){
            timer->stop();
            ChangePasswordDialog psw;
            psw.exec();
            timer->start(UPDATE_TIME);
        }
        else QMessageBox::information(this,"Informacja","Nie połączono z bazą danych.");
    });
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
    connect(backupButton, &QPushButton::clicked,[=](){
        if(isDatabase)
            createBackup();
        else
            QMessageBox::information(this,"Informacja","Nie połączono z bazą danych.");
    });

    setBackupButtonVisible();
}

void MainWindow::createDBConfigButton()
{
    dbConfigButton = new QPushButton(this);
    dbConfigButton->setIcon(QIcon(":/images/images/dbconfig.png"));
    dbConfigButton->setToolTip("Konfiguracja bazy danych");
    dbConfigButton->setStyleSheet("border:none; color: gray");
    ui->statusBar->addPermanentWidget(dbConfigButton);
    connect(dbConfigButton, &QPushButton::clicked,[=](){
        QString line{};
        timer->stop();
        bool result = false ;

        if(isAdmin){
            QMessageBox msgBox(QMessageBox::Question, tr("Wybierz!"), tr("<font face=""Calibri"" size=""3"" color=""gray"">Chcesz utworzyć czy połączyć z bazą danych?</font>"), QMessageBox::Close);
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
                                 "min-width: 70px;"
                                 "min-height: 30px;"
                                 "font-family: Calibri;"
                                 "font-size: 14;"
                                 "font-weight: bold;"
                                 "}"
                                 "QPushButton:pressed {"
                                 "color: white;"
                                 "border-radius: 5px;"
                                 "background: rgb(255,105,0);"
                                 "}"
                                 );

            msgBox.setWindowIcon(QIcon(":/images/images/icon.ico"));
            QAbstractButton *myYesButton = msgBox.addButton(trUtf8("Połącz"), QMessageBox::YesRole);
            QAbstractButton *myNoButton = msgBox.addButton(trUtf8("Dodaj/Usuń"), QMessageBox::NoRole);
            msgBox.setButtonText(QMessageBox::Close, tr("Zamknij"));
            msgBox.exec();

            if(msgBox.clickedButton() == myNoButton)
                result = true;
            else if (msgBox.clickedButton() == myYesButton)
                result = false;
            else {
                timer->start(UPDATE_TIME);
                return;
            }
        }

        if(!DBConfigDialog::readFromFile(line)){
            timer->start(UPDATE_TIME);
            return;
        }

        DBConfigDialog d(line,result);

        connect(&d,SIGNAL(connectedToDB(bool)),this,SLOT(updateView(bool)),Qt::QueuedConnection);
        connect(&d,SIGNAL(changeStatusBar(QString)),statusLabel,SLOT(setText(QString)));
        d.exec();
        loadTrayIcon();
        timer->start(UPDATE_TIME);
    });
}

void MainWindow::setBackupButtonVisible()
{
    if(!isAdmin){
        backupButton->setVisible(false);
        changePSW->setVisible(false);
    }
    if(isAdmin){
        backupButton->setVisible(true);
        changePSW->setVisible(true);
    }
}

void MainWindow::createLoginOption()
{
    QPushButton * logoutButton = new QPushButton(this);
    logoutButton->setIcon(QIcon(":/images/images/exit.png"));
    logoutButton->setStyleSheet("border:none;");
    logoutButton->setToolTip("Wyloguj");
    logoutButton->setVisible(false);

    QPushButton * loginButton = new QPushButton(this);
    loginButton->setIcon(QIcon(":/images/images/key.png"));
    loginButton->setStyleSheet("border:none;");
    loginButton->setToolTip("Zaloguj jako administrator");

    QLineEdit *  adminPassword = new QLineEdit(this);
    adminPassword->setVisible(true);
    adminPassword->setEchoMode(QLineEdit::Password);
    adminPassword->setFixedWidth(100);
    adminPassword->setPlaceholderText("Admin");

    ui->statusBar->addPermanentWidget(logoutButton);
    ui->statusBar->addPermanentWidget(adminPassword);
    ui->statusBar->addPermanentWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, [=]() {
        if(!isDatabase){
            if(adminPassword->text()==ADMIN_PASSWD) {
                isAdmin = true;
                setBackupButtonVisible();
                loginButton->setVisible(false);
                logoutButton->setVisible(true);
                adminPassword->clear();
                adminPassword->setVisible(false);
                return;
            }
        }

        else {
            this->setEnabled(false);
            qApp->processEvents();
            if(Database::isOpen()) {
                QSqlQueryModel *pswQry = new QSqlQueryModel(this);
                pswQry->setQuery(QString("SELECT Password FROM admin WHERE idAdmin=0"));
                const QString admin_password = pswQry->data(pswQry->index(0,0)).toString();
                qDebug() << admin_password;
                delete pswQry;
                if(adminPassword->text()==admin_password) {
                    isAdmin = true;
                    setBackupButtonVisible();
                    loginButton->setVisible(false);
                    logoutButton->setVisible(true);
                    adminPassword->setVisible(false);
                    loadTrayIcon();
                    updateView(false);
                }
            }
            else {
                QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
                //ui->statusBar->showMessage("Nie można połączyć z bazą danych");
                statusLabel->setText("Nie można połączyć z bazą danych");
            }
            this->setEnabled(true);
            qApp->processEvents();
        }
    });

    connect(logoutButton, &QPushButton::clicked, [=]() {

        if(!isDatabase){
            isAdmin = false;
            setBackupButtonVisible();
            loginButton->setVisible(true);
            logoutButton->setVisible(false);
            adminPassword->setVisible(true);
            adminPassword->clear();
            return;
        }

        this->setEnabled(false);
        qApp->processEvents();
            if(Database::isOpen()) {
                isAdmin = false;
                updateView(true);
                loadTrayIcon();
                setBackupButtonVisible();
                loginButton->setVisible(true);
                logoutButton->setVisible(false);
                adminPassword->setVisible(true);
                adminPassword->clear();
            }
            else {
                QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
                //ui->statusBar->showMessage("Nie można połączyć z bazą danych");
                statusLabel->setText("Nie można połączyć z bazą danych");
            }
        this->setEnabled(true);
        qApp->processEvents();
    });
}

// ----- TRAY ICON METHODS--------------

void MainWindow::setVisible(bool visible)
{
    QMainWindow::setVisible(visible);

    if(isDatabase){
        minimizeAction->setEnabled(visible);
        restoreAction->setEnabled(!visible);

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
}

void MainWindow::showTrayIcon()
{
    trayIcon->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isDatabase) {
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
                                 "font-size: 14;"
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
    if(Database::isOpen()) {
        reloadNotes();

        delete trayIcon;
        delete trayIconMenu;

        createActions(isAdmin);
        createTrayIcon(isAdmin);
        setIcon();
        showTrayIcon();
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
        connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(poupMessageClicked()));
    }
    else statusLabel->setText("Nie można połączyć z bazą danych"); // ui->statusBar->showMessage("Nie można połączyć z bazą danych");
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
    if(Database::isOpen()) {
        notesTable->setQuery("SELECT * FROM notes WHERE isRead = 0 ORDER BY Datetime DESC, idNotes DESC;");
        emit trayMenuNoteClicked(notesTable->data(notesTable->index(0,0)).toInt(), notesTable->data(notesTable->index(0,6)).toInt());
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

