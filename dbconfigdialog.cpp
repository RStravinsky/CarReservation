#include "dbconfigdialog.h"
#include "ui_dbconfigdialog.h"
#include "database.h"

DBConfigDialog::DBConfigDialog(QString line, bool noDB, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBConfigDialog),
    noDataBase(noDB)
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
        ui->lePort->setText(parameters.at(1));
    }

    if(noDataBase) {
        ui->runButton->setText("Utwórz");
    }
    else
        ui->runButton->setText("Połącz");

    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

DBConfigDialog::~DBConfigDialog()
{
    delete ui;
}

void DBConfigDialog::on_runButton_clicked()
{
    bool isLocal;
    // show waiting labels
    this->setEnabled(false);
    ui->lblWait->setVisible(true);
    ui->lblLoad->setVisible(true);
    qApp->processEvents();

    if(ui->rbLocalDB->isChecked()) {
        Database::purgeDatabase();
        if(noDataBase) {
            qDebug() << "Create local db";
            QString pth = QString(QDir::currentPath() + "\\\"");
            QString cmd = QString("\"" + pth + "mysqlRun -h127.0.0.1 -P3306 -uroot -pPASSWORD");
            QProcess * createMysql = new QProcess(this);
            createMysql->start(cmd);
            while(!createMysql->waitForFinished()) {}
            delete createMysql;
        }

        Database::setParameters("localhost", 3306,"sigmacars", "root","PASSWORD");
        if(!writeToFile("localhost", 3306, "sigmacars", "root","PASSWORD"))
            return;
        isLocal = true;
    }

    else if (ui->rbRemoteDB->isChecked()) {

        if(dataIsEmpty())
            return;

        Database::purgeDatabase();

        if(noDataBase) {
            qDebug() << "Create remote db";
            QString pth = QString(QDir::currentPath() + "\\\"");
            QString cmd = QString("\"" + pth + "mysqlRun -h" + ui->leAddress->text() + " -P" + ui->lePort->text() + " -u" + ui->leUser->text() + " -p" + ui->lePassword->text() + " -dtestsigmadb");
            QProcess * createMysql = new QProcess(this);
            createMysql->start(cmd);
            while(!createMysql->waitForFinished()) {}
            delete createMysql;
        }

        Database::setParameters(ui->leAddress->text(), ui->lePort->text().toInt(),
                                "testsigmadb", ui->leUser->text(),
                                ui->lePassword->text());
        if(!writeToFile(ui->leAddress->text(), ui->lePort->text().toInt(),
                                "testsigmadb", ui->leUser->text(),ui->lePassword->text()))
            return;
        isLocal = false;
    }

    if(Database::connectToDatabase()) {
        QMessageBox::information(this,"Informacja", "Pomyślnie połączono z bazą danych.");
        emit connectedToDB(false);
        ui->lblWait->setVisible(false);
        ui->lblLoad->setVisible(false);
        this->setEnabled(true);
        MainWindow::isDatabase = true;
        Database::isLocal = isLocal;
        this->accept();
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Nie połączono z bazą danych!");
        ui->lblWait->setVisible(false);
        ui->lblLoad->setVisible(false);
        this->setEnabled(true);
        emit changeStatusBar("Nie można połączyć z bazą danych");
        MainWindow::isDatabase = false;
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
        if(noDataBase)
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
        if(noDataBase)
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

        //unlock
        ui->lblWait->setVisible(false);
        ui->lblLoad->setVisible(false);
        this->setEnabled(true);

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

        //unlock
        ui->lblWait->setVisible(false);
        ui->lblLoad->setVisible(false);
        this->setEnabled(true);

        return true;
    }

    return false;
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
        msgBox.exec();
        return false;
    }
    QTextStream in(&initFile);
    line = in.readLine();
    initFile.close();

    return true;
}
