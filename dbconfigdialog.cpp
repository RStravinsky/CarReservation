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

    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

DBConfigDialog::~DBConfigDialog()
{
    delete ui;
}

void DBConfigDialog::on_runButton_clicked()
{
    Database::purgeDatabase();

    if(ui->rbLocalDB->isChecked()) {
        if(!noDataBase) {
            Database::setParameters("localhost", 3306,"sigmacars", "root","PASSWORD");
            writeToFile("localhost", 3306, "sigmacars", "root","PASSWORD");
        }
        else {
            // create local Data base
            Database::setParameters("localhost", 3306,"sigmacars", "root","PASSWORD");
            writeToFile("localhost", 3306, "sigmacars", "root","PASSWORD");
        }
    }

    else if (ui->rbRemoteDB->isChecked()) {
        if(!noDataBase) {
            Database::setParameters(ui->leAddress->text(), ui->lePort->text().toInt(),
                                    "sigmacars", ui->leUser->text(),
                                    ui->lePassword->text());
            writeToFile(ui->leAddress->text(), ui->lePort->text().toInt(),
                        "sigmacars", ui->leUser->text(),
                        ui->lePassword->text());
        }
        else {
            // create remote Data base
        }
    }

    if(Database::connectToDatabase()) {
        QMessageBox::information(this,"Informacja", "Pomyślnie połączono z bazą danych.");
        emit connectedToDB(false);
        this->accept();
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        this->reject();
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
    }
}

bool DBConfigDialog::writeToFile(const QString &hostname, int port, const QString &database, const QString &user, const QString &password)
{
    QFile initFile( QDir::currentPath()+"/init.txt" );
    if (!initFile.open(QIODevice::WriteOnly))
            return false;

    QTextStream out( &initFile );
    out << hostname << ";" << QString::number(port) << ";" << database << ";" << user << ";" << password;
    initFile.close();
}
