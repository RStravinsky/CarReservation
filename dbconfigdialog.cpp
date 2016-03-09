#include "dbconfigdialog.h"
#include "ui_dbconfigdialog.h"
#include "database.h"

DBConfigDialog::DBConfigDialog(QString line, bool isCreateType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBConfigDialog),
    createMode(isCreateType)
{
    ui->setupUi(this);

    ui->lblAddress->setVisible(false);
    ui->leAddress->setVisible(false);
    ui->lblUser->setVisible(false);
    ui->leUser->setVisible(false);
    ui->lblPassword->setVisible(false);
    ui->lePassword->setVisible(false);
    ui->lblPort->setVisible(false);
    ui->lePort->setVisible(false);
    ui->lblWait->setVisible(false);
    ui->lblLoad->setVisible(false);

    if(!line.isEmpty()) {
        QStringList parameters = line.split(";");
        ui->leUser->setText(parameters.at(3));
        ui->lePassword->setText(parameters.at(4));
        ui->leAddress->setText(parameters.at(0));
        currentAddress = parameters.at(0);
        ui->lePort->setText(parameters.at(1));
    }

    if(createMode) {
        ui->runButton->setText("Utwórz");
        this->setWindowTitle("Kreator bazy danych");
    }
    else {
        ui->runButton->setText("Połącz");
        this->setWindowTitle("Połącz z bazą danych");
         ui->deleteButton->setVisible(false);
    }

    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

DBConfigDialog::~DBConfigDialog()
{
    delete ui;
}

bool DBConfigDialog::createDatabase(QString command)
{
    QString cmd = QString(command);
    QProcess * createMysql = new QProcess(this);
    createMysql->start(cmd);
    while(!createMysql->waitForFinished()) {}
    int result = createMysql->exitCode();
    delete createMysql;

    return result;
}


void DBConfigDialog::on_runButton_clicked()
{
    bool isLocal;
    bool isDbExist = false;
    setGrayOut(true);

    if(ui->rbLocalDB->isChecked()) {
        Database::purgeDatabase();
        if(createMode){
            if(createDatabase("\"" + QDir::currentPath() + "\\\"" + "mysqlRun -h127.0.0.1 -P3306 -uroot -pPASSWORD")) {
                QMessageBox::information(this,"Informacja", "Baza danych już istnieje.");
                isDbExist = true;
            }
        }
        Database::setParameters("127.0.0.1", 3306,"testsigmadb", "root","PASSWORD");
        isLocal = true;
    }

    else if (ui->rbRemoteDB->isChecked()) {

        if(dataIsEmpty()){
            setGrayOut(false);
            return;
        }

        Database::purgeDatabase();
        if(createMode){
            if(createDatabase("\"" + QDir::currentPath() + "\\\"" + "mysqlRun -h" + ui->leAddress->text() + " -P" + ui->lePort->text() + " -u" + ui->leUser->text() + " -p" + ui->lePassword->text() + " -dtestsigmadb")) {
                QMessageBox::information(this,"Informacja", "Baza danych już istnieje.");
                isDbExist = true;
            }
        }
        Database::setParameters(ui->leAddress->text(), ui->lePort->text().toInt(),"testsigmadb", ui->leUser->text(),ui->lePassword->text());
        isLocal = false;
    }

    if(Database::connectToDatabase()) {
        if(!createMode)QMessageBox::information(this,"Informacja", "Pomyślnie połączono z bazą danych.");
        else if(isDbExist)
            QMessageBox::information(this,"Informacja", "Pomyślnie połączono z bazą danych.");
        else
            QMessageBox::information(this,"Informacja", "Pomyślnie dodano bazę danych.");

        if(isLocal) {
            if(!writeToFile("127.0.0.1", 3306, "testsigmadb", "root","PASSWORD"))
                return;
        }
        else {
            if(!writeToFile(ui->leAddress->text(), ui->lePort->text().toInt(),
                    "testsigmadb", ui->leUser->text(),ui->lePassword->text()))
                return;
        }

        setGrayOut(false);
        MainWindow::isDatabase = true;
        Database::isLocal = isLocal;
        emit connectedToDB(false);
        this->accept();
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Nie połączono z bazą danych!");
        setGrayOut(false);
        MainWindow::isDatabase = false;
        emit changeStatusBar("Nie można połączyć z bazą danych");
        return;
    }
}

void DBConfigDialog::on_rbRemoteDB_toggled(bool checked)
{
    if(checked) {
        ui->lblAddress->setVisible(true);
        ui->leAddress->setVisible(true);
        ui->lblUser->setVisible(true);
        ui->leUser->setVisible(true);
        ui->lblPassword->setVisible(true);
        ui->lePassword->setVisible(true);
        ui->lblPort->setVisible(true);
        ui->lePort->setVisible(true);
        if(createMode)
            ui->runButton->setText("Utwórz");
        else
            ui->runButton->setText("Połącz");
    }
    else {
        ui->lblAddress->setVisible(false);
        ui->leAddress->setVisible(false);
        ui->lblUser->setVisible(false);
        ui->leUser->setVisible(false);
        ui->lblPassword->setVisible(false);
        ui->lePassword->setVisible(false);
        ui->lblPort->setVisible(false);
        ui->lePort->setVisible(false);
        if(createMode)
            ui->runButton->setText("Utwórz");
        else
            ui->runButton->setText("Połącz");
    }
}

bool DBConfigDialog::writeToFile(const QString &hostname, int port, const QString &database, const QString &user, const QString &password)
{
    QFile initFile( QDir::currentPath()+"/init.txt" );
    if(!initFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this,"Błąd!", "Nie można otworzyć pliku z kofiguracją bazy danych.");
        setGrayOut(false);
        return false;
    }
    QTextStream out( &initFile );
    out << hostname << ";" << QString::number(port) << ";" << database << ";" << user << ";" << password;
    initFile.close();

    return true;
}

bool DBConfigDialog::dataIsEmpty()
{
    if(ui->leUser->text().isEmpty() | ui->lePassword->text().isEmpty() || ui->leAddress->text().isEmpty() || ui->lePort->text().isEmpty()) {
        QMessageBox::warning(this,"Uwaga!","Pole tekstowe nie zostało wypełnione.");
        return true;
    }

    return false;
}

void DBConfigDialog::setGrayOut(bool isGray)
{
    ui->lblWait->setVisible(isGray);
    ui->lblLoad->setVisible(isGray);
    this->setEnabled(!isGray);
    qApp->processEvents();
}

bool DBConfigDialog::readFromFile(QString &line)
{
    QFile initFile(QDir::currentPath()+"/init.txt");
    if (!initFile.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox(QMessageBox::Critical, QString("Błąd!"), QString("<font face=""Calibri"" size=""3"" color=""gray"">Nie można otworzyć pliku konfiguracji bazy danych.</font>"));
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
        msgBox.exec();
        return false;
    }
    QTextStream in(&initFile);
    line = in.readLine();
    initFile.close();

    return true;
}

void DBConfigDialog::on_deleteButton_clicked()
{
    if(currentAddress.isEmpty()){
        QMessageBox::information(this,"Informacja","Nie połączono z bazą danych.");
        return;
    }

    if(ui->rbRemoteDB->isChecked() && currentAddress=="127.0.0.1"){
        QMessageBox::information(this,"Informacja","Aby usunąć zdalną bazę danych proszę sie przelogować.");
        return;
    }

    else if (ui->rbLocalDB->isChecked() && currentAddress!="127.0.0.1"){
        QMessageBox::information(this,"Informacja","Aby usunąć lokalną bazę danych proszę sie przelogować.");
        return;
    }


    if(showMsgBeforeDelete()) {
        setGrayOut(true);

        QSqlQuery qry;
        qry.prepare("DROP DATABASE testsigmadb");
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Usuwanie nie powiodło się.\nERROR: "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Usunieto!");
            QFile initFile1( QDir::currentPath()+"/init.txt" );
            initFile1.remove();
            QFile initFile2( QDir::currentPath()+"/init.txt" );
            if (!initFile2.open(QIODevice::WriteOnly))
                QMessageBox::critical(this,"Błąd!", "Nie można otworzyć pliku z kofiguracją bazy danych.");

            currentAddress.clear();
            MainWindow::isDatabase = false;
            ui->leUser->clear();
            ui->lePassword->clear();
            ui->leAddress->clear();
            ui->lePort->clear();
            emit changeStatusBar("Nie można połączyć z bazą danych");
        }

        setGrayOut(false);
    }
}

bool DBConfigDialog::showMsgBeforeDelete()
{
    QMessageBox msgBox(QMessageBox::Question, tr("Usuwanie bazy danych!"), "<font face=""Calibri"" size=""3"" color=""gray"">Czy na pewno chcesz usunąć bazę danych:\nNazwa: testsigmadb \nAdres: "+currentAddress+"</font>", QMessageBox::Yes | QMessageBox::No );
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
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Nie"));
    if (msgBox.exec() == QMessageBox::No)
        return false;

    return true;
}

