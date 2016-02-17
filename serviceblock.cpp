#include "serviceblock.h"
#include "ui_serviceblock.h"

ServiceBlock::ServiceBlock(int idC, QWidget *parent) :
    QDialog(parent),
    idCar(idC),
    ui(new Ui::ServiceBlock)
{
    ui->setupUi(this);

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

    for(int i=0; i<listModel->rowCount(); ++i)
        nameList << listModel->data(listModel->index(i,0)).toString();
    nameListModel.setStringList(nameList);
    ui->lvServices->setModel(&nameListModel);

    isOpen = true;
}

ServiceBlock::~ServiceBlock()
{
    isOpen = false;
    delete ui;
}

bool ServiceBlock::isOpen;

void ServiceBlock::on_lvServices_clicked(const QModelIndex &index)
{
    if(Database::connectToDatabase("rezerwacja","rezerwacja")) {
        serviceTable = new QSqlQueryModel(this);
        serviceTable->setQuery(QString("SELECT * FROM service WHERE idCar = %1 ORDER BY EventDate DESC;").arg(idCar));
        ui->cbName->setCurrentText( serviceTable->index(index.row(),1).data().toString() );
        ui->cbCategory->setCurrentText( serviceTable->index(index.row(),2).data().toString() );
        ui->deEvent->setDate( serviceTable->index(index.row(),3).data().toDate() );
        ui->deGuarantee->setDate( serviceTable->index(index.row(),4).data().toDate() );
        ui->deBegin->setDate( serviceTable->index(index.row(),5).data().toDate() );
        ui->deEnd->setDate( serviceTable->index(index.row(),6).data().toDate() );
        ui->leCost->setText( serviceTable->index(index.row(),7).data().toString() );
        ui->teNotes->setText( serviceTable->index(index.row(),8).data().toString() );
    }
    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
    }
}
