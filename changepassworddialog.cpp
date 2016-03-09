#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"
#include "database.h"

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
    ui->leOldPassword->setFocus();

    pswQry = new QSqlQueryModel(this);
    pswQry->setQuery(QString("SELECT Password FROM admin WHERE idAdmin=0"));
    oldPassword = pswQry->data(pswQry->index(0,0)).toString();
    delete pswQry;
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

bool ChangePasswordDialog::dataIsEmpty()
{
    if(ui->leNewPassword->text().isEmpty() | ui->leNewPasswordAgain->text().isEmpty() || ui->leOldPassword->text().isEmpty()) {
        QMessageBox::warning(this,"Uwaga!","Pole tekstowe nie zostało wypełnione.");
        return true;
    }
    return false;
}

void ChangePasswordDialog::on_pushButtonConfirm_clicked()
{
    if(dataIsEmpty())
        return;

    if(ui->leNewPassword->text() != ui->leNewPasswordAgain->text()){
        QMessageBox::warning(this,"Uwaga!","Wprowadzone hasła różnią się.");
        return;
    }

    if(ui->leOldPassword->text() != oldPassword){
        QMessageBox::warning(this,"Uwaga!","Aktualne hasło jest nie poprawne.");
        return;
    }

    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("UPDATE admin SET Password=:_Password WHERE idAdmin=0");
        qry.bindValue(":_Password", ui->leNewPasswordAgain->text());
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powiodła się.\nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
}
