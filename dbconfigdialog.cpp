#include "dbconfigdialog.h"
#include "ui_dbconfigdialog.h"
#include "database.h"

DBConfigDialog::DBConfigDialog(bool noDB, QWidget *parent) :
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

    if(noDataBase)
        ui->runButton->setText("Utwórz");
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
    if(ui->rbLocalDB->isChecked()) {
        Database::purgeDatabase();
        if(noDataBase) {

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
    }

    else if (ui->rbRemoteDB->isChecked()) {

        if(dataIsEmpty())
            return;

        Database::purgeDatabase();

        if(noDataBase) {

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
    }

    if(Database::connectToDatabase()) {
        QMessageBox::information(this,"Informacja", "Pomyślnie połączono z bazą danych.");
        emit connectedToDB(false);
        this->accept();
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Nie połączono z bazą danych!");
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
