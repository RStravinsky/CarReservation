#include "dbconfigdialog.h"
#include "ui_dbconfigdialog.h"
#include "database.h"

DBConfigDialog::DBConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBConfigDialog)
{
    ui->setupUi(this);
}

DBConfigDialog::~DBConfigDialog()
{
    delete ui;
}

void DBConfigDialog::on_cancelButton_clicked()
{
    this->close();
}

void DBConfigDialog::on_runButton_clicked()
{
    Database::purgeDatabase();
    Database::setParameters(ui->leAddress->text(), ui->lePort->text().toInt(),
                               ui->leDatabase->text(), ui->leUser->text(),
                               ui->lePassword->text());

    Database::connectToDatabase();
    this->close();
}
