#include "serviceblock.h"
#include "ui_serviceblock.h"

ServiceBlock::ServiceBlock(int idC, QWidget *parent) :
    QDialog(parent),
    idCar(idC),
    ui(new Ui::ServiceBlock)
{
    ui->setupUi(this);

    if(Database::connectToDatabase("rezerwacja", "rezerwacja")) {

        QSqlQueryModel * windTitle = new QSqlQueryModel(this);
        windTitle->setQuery(QString("SELECT Brand, Model, LicensePlate FROM sigmacars.car WHERE idCar = %1").arg(idCar));
        this->setWindowTitle( QString("Naprawy - ")
                              + windTitle->data(windTitle->index(windTitle->rowCount()-1,0)).toString() + QString(" ")
                              + windTitle->data(windTitle->index(windTitle->rowCount()-1,1)).toString() + QString(" - ")
                              + windTitle->data(windTitle->index(windTitle->rowCount()-1,2)).toString()
                              );

        serviceTable = new QSqlQueryModel(this);
        serviceTable->setQuery(QString("SELECT * FROM service WHERE idCar = %1 ORDER BY EventDate DESC;").arg(idCar));
        listModel = new QSqlQueryModel(this);
        listModel->setQuery(QString("SELECT Name FROM service WHERE idCar = %1 ORDER BY EventDate DESC;").arg(idCar));
        ui->lvServices->setModel(listModel);
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Nie można połączyć z bazą danych!");
    }
}

ServiceBlock::~ServiceBlock()
{
    delete ui;
}

void ServiceBlock::on_lvServices_clicked(const QModelIndex &index)
{
    ui->cbName->setCurrentText( serviceTable->index(index.row(),1).data().toString() );
    ui->cbCategory->setCurrentText( serviceTable->index(index.row(),2).data().toString() );
    ui->deEvent->setDate( serviceTable->index(index.row(),3).data().toDate() );
    ui->deGuarantee->setDate( serviceTable->index(index.row(),4).data().toDate() );
    ui->deBegin->setDate( serviceTable->index(index.row(),5).data().toDate() );
    ui->deEnd->setDate( serviceTable->index(index.row(),6).data().toDate() );
    ui->leCost->setText( serviceTable->index(index.row(),7).data().toString() );
    ui->teNotes->setText( serviceTable->index(index.row(),8).data().toString() );
}
