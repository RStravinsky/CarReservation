#include "carblock.h"
#include "ui_carblock.h"


CarBlock::CarBlock(int id, QString name, QString model, QString licensePlate, QDate inspectionDate, QDate insuranceDate, int mileage, Status status, QString photoPath, bool toAdd, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarBlock),
    idCar(id)
{
    ui->setupUi(this);

    isAddBlock = toAdd;
    if(isAddBlock){
        ui->btnReserve->setVisible(false);
        ui->btnAddInspection->setVisible(false);
        ui->btnAddInsurance->setVisible(false);
        ui->lblStatus->setVisible(false);
        ui->lblMileage->setReadOnly(false);
        ui->lblCarName->setReadOnly(false);
        ui->lblLicensePlate->setReadOnly(false);
        ui->btnAddImage->setVisible(true);
        ui->btnRemove->setIcon(QIcon(":/images/images/add.png"));
    }

    ui->lblLicensePlate->setText(licensePlate);
    ui->lblPhoto->setPixmap(QPixmap(photoPath));
    ui->lblCarName->setText(name + QString(" ") + model);
    ui->lblMileage->setText(QString::number(mileage) + " km");
    ui->dateEditInspection->setDate(inspectionDate);
    ui->dateEditInsurance->setDate(insuranceDate);
    setStatus(status);

}

CarBlock::~CarBlock()
{
    delete ui;
}

void CarBlock::setStatus(Status status)
{
    if(status == Status::Free)
        ui->lblStatus->setPixmap(QPixmap(":/images/images/free.png"));
    if(status == Status::Rented)
        ui->lblStatus->setPixmap(QPixmap(":/images/images/rented.png"));
}

void CarBlock::setAdminPermissions(bool isAdmin)
{
    if(!isAdmin) {
        ui->lblInspectionImage->setVisible(false);
        ui->lblInsuranceImage->setVisible(false);
        ui->lblNotesImage->setVisible(false);
        ui->dateEditInspection->setVisible(false);
        ui->dateEditInsurance->setVisible(false);
        ui->btnAddInspection->setVisible(false);
        ui->btnAddInsurance->setVisible(false);
        ui->btnViewNotes->setVisible(false);
        ui->btnRemove->setVisible(false);
        ui->btnAddImage->setVisible(false);
    }
    else {
        ui->lblInspectionImage->setVisible(true);
        ui->lblInsuranceImage->setVisible(true);
        ui->lblNotesImage->setVisible(true);
        ui->dateEditInspection->setVisible(true);
        ui->dateEditInsurance->setVisible(true);
        ui->btnAddInspection->setVisible(true);
        ui->btnAddInsurance->setVisible(true);
        ui->btnViewNotes->setVisible(true);
        ui->btnRemove->setVisible(true);
        ui->btnAddImage->setVisible(false);
    }
}

void CarBlock::showNotesDialog(int _idNotes, int _idCar)
{
    Q_UNUSED(_idNotes);

    if(_idCar == idCar) {
    NotesDialog n(_idCar);
    n.exec();
    }
}

void CarBlock::on_btnReserve_clicked()
{
    bookingDialog = new BookingDialog(bookingTable, carTable, idCar);
    bookingDialog->exec();
    delete bookingDialog;
}

void CarBlock::on_btnAddInsurance_clicked()
{
    QSqlQuery qry;
    qry.prepare("UPDATE car SET InsuranceDate=:_insuranceDate WHERE idCar=:_id");
    qry.bindValue(":_id", idCar);
    qry.bindValue(":_insuranceDate", ui->dateEditInsurance->date());
    if( !qry.exec() )
        QMessageBox::warning(this,"Informacja","Aktualizacja nie powidoła się./nERROR: "+qry.lastError().text()+"");
    else {
        QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
}

void CarBlock::on_btnAddInspection_clicked()
{
    QSqlQuery qry;
    qry.prepare("UPDATE car SET InspectionDate=:_inspectionDate WHERE idCar=:_id");
    qry.bindValue(":_id", idCar);
    qry.bindValue(":_inspectionDate", ui->dateEditInspection->date());
    if( !qry.exec() )
        QMessageBox::warning(this,"Informacja","Aktualizacja nie powidoła się./nERROR: "+qry.lastError().text()+"");
    else {
        QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
}

void CarBlock::on_btnRemove_clicked()
{
    if(!isAddBlock) {
        QSqlQuery qry;
        qry.prepare("DELETE FROM car WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        if( !qry.exec() )
            QMessageBox::warning(this,"Informacja","Usuwanie nie powiodła się./nERROR: "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Usunieto!");
            emit carDeleted();
        }
    }

    else {
        QSqlQuery qry;
        qry.prepare("INSERT INTO car(Brand,Model,LicensePlate,InspectionDate,"
                                    "InsuranceDate,Mileage,Status,PhotoPath)"
                     "VALUES(:_Brand,:_Model,:_LicensePlate,:_InspectionDate,"
                     ":_InsuranceDate,:_Mileage,:_Status,:_PhotoPath)");

        qry.bindValue(":_Brand", ui->lblCarName->text().split(" ").at(0));
        qry.bindValue(":_Model", ui->lblCarName->text().split(" ").at(1));
        qry.bindValue(":_LicensePlate", ui->lblLicensePlate->text());
        qry.bindValue(":_InspectionDate",ui->dateEditInspection->date());
        qry.bindValue(":_InsuranceDate",ui->dateEditInsurance->date());
        qry.bindValue(":_Status",0);
        qry.bindValue(":_Mileage", ui->lblMileage->text());
        qry.bindValue(":_PhotoPath",addedCarImagePath);
        if( !qry.exec() )
            QMessageBox::warning(this,"Informacja","Dodawanie nie powiodło się./nERROR "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Dodano!");
            emit carAdded();
        }
    }
}

void CarBlock::on_btnViewNotes_clicked()
{
    idNotes = 0; // <- add idNotes initialization in class constructor
    showNotesDialog(idNotes, idCar);

}

void CarBlock::on_btnAddImage_clicked()
{
    addedCarImagePath = QFileDialog::getOpenFileName(this, tr("Plik z obrazkiem"),
                               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/bez_tytułu.pdf",
                               tr("Pliki PNG (*.png)"));
    ui->lblPhoto->setPixmap(QPixmap(addedCarImagePath));

}
