#include "oildialog.h"
#include "ui_oildialog.h"

OilDialog::OilDialog(int id,QWidget *parent):
    QDialog(parent),
    ui(new Ui::OilDialog),
    idCar(id )
{
    ui->setupUi(this);
    QSqlQueryModel * windTitle = new QSqlQueryModel(this);
    windTitle->setQuery(QString("SELECT Brand, Model, LicensePlate,Oil FROM car WHERE idCar = %1").arg(idCar));
    this->setWindowTitle( QString("Wymiana oleju - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,0)).toString() + QString(" ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,1)).toString() + QString(" - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,2)).toString()
                          );
    ui->leOil->setText(windTitle->data(windTitle->index(windTitle->rowCount()-1,3)).toString());
    delete windTitle;
}

OilDialog::~OilDialog()
{
    delete ui;
}

void OilDialog::on_btnAddOil_clicked()
{
    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET Oil=:_Oil WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_Oil", ui->leOil->text());
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powiodła się.\nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
}


