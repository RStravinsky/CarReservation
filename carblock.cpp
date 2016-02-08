#include "carblock.h"
#include "ui_carblock.h"

extern std::shared_ptr<NotesDialog> notesDialogPointer;

CarBlock::CarBlock( bool toAdd,int id, QString name, QString model, QString licensePlate, QDate inspectionDate, QDate insuranceDate, int mileage, Status status,
                   QString photoPath, QWidget *parent):
    QWidget(parent),
    ui(new Ui::CarBlock),
    idCar(id),
    isAddBlock(toAdd)
{
    ui->setupUi(this);

    if(!isAddBlock){
        ui->lblLicensePlate->setText(licensePlate);
        ui->lblCarName->setText(name + QString(" ") + model);
        ui->lblMileage->setText(QString::number(mileage) + " km");
    }

    else if(isAddBlock){
        ui->btnReserve->setVisible(false);
        ui->btnAddInspection->setVisible(false);
        ui->btnAddInsurance->setVisible(false);
        ui->lblStatus->setVisible(false);
        ui->btnViewNotes->setVisible(false);
        ui->lblNotesImage->setVisible(false);
        ui->btnAddImage->setVisible(true);
        ui->lblMileage->setReadOnly(false);
        ui->lblCarName->setReadOnly(false);
        ui->lblLicensePlate->setReadOnly(false);
        ui->btnRemove->setIcon(QIcon(":/images/images/add.png"));
        QRegExp rx("^\\w+\\s\\w+$");
        ui->lblCarName->setValidator(new QRegExpValidator(rx, this));
    }

    ui->lblPhoto->setPixmap(QPixmap(photoPath));
    ui->dateEditInspection->setDate(inspectionDate);
    ui->dateEditInsurance->setDate(insuranceDate);
    setStatus(status);
}

CarBlock::CarBlock(CarBlock &block, QWidget *parent):
    QWidget(parent),
    ui(new Ui::CarBlock)
{
    ui->setupUi(this);
    ui->btnReserve->setVisible(false);
    ui->btnAddInspection->setVisible(false);
    ui->btnAddInsurance->setVisible(false);
    ui->lblStatus->setVisible(false);
    ui->btnViewNotes->setVisible(false);
    ui->lblNotesImage->setVisible(false);
    ui->btnAddImage->setVisible(true);
    ui->lblMileage->setReadOnly(false);
    ui->lblCarName->setReadOnly(false);
    ui->lblLicensePlate->setReadOnly(false);
    ui->btnRemove->setIcon(QIcon(":/images/images/add.png"));

    isAddBlock = true;
    ui->lblPhoto->setPixmap(block.addedCarImagePath);
    if(block.addedCarImagePath.isEmpty())
        ui->lblPhoto->setPixmap(QPixmap(":/images/images/car.png"));
    else {
        ui->lblPhoto->setPixmap(block.addedCarImagePath);
        addedCarImagePath = block.addedCarImagePath;
    }
    ui->lblCarName->setText(block.getCarName());
    ui->lblMileage->setText(block.getMileage());
    ui->lblLicensePlate->setText(block.getLicensePlate());
    ui->dateEditInspection->setDate(block.getDates().first);
    ui->dateEditInsurance->setDate(block.getDates().second);
    setStatus(Status::Free);

    QRegExp rx("^\\w+\\s\\w+$");
    ui->lblCarName->setValidator(new QRegExpValidator(rx, this));
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
    if(_idCar == idCar) {
        if(Database::getDatabase().isOpen())
            Database::closeDatabase();
        if(Database::connectToDatabase("root","Serwis4q@")) {
            emit inProgress();
                notesDialogPointer = std::shared_ptr<NotesDialog>(new NotesDialog(_idNotes, _idCar));
                if(notesDialogPointer.use_count() == 1 && notesDialogPointer.get()->exec() == NotesDialog::Rejected){
                    Database::closeDatabase();
                    emit progressFinished();
                    emit noteClosed();
                    qDebug() << "CarBlock showNotesDialog - noteClosed emmited";
                }

        }
        else {
            Database::closeDatabase();
            QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        }
    }
}

void CarBlock::on_btnReserve_clicked()
{
    if(Database::connectToDatabase("root","Serwis4q@")) {
        bookingDialog = new BookingDialog(bookingTable, carTable, idCar);
        emit inProgress();
        if(bookingDialog->exec()== BookingDialog::Rejected)
            emit progressFinished();
        delete bookingDialog;
        Database::closeDatabase();
    }
    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }
}

void CarBlock::on_btnAddInsurance_clicked()
{
    emit inProgress();

    if(Database::connectToDatabase("root","Serwis4q@")) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET InsuranceDate=:_insuranceDate WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_insuranceDate", ui->dateEditInsurance->date());
        bool isExecuted = qry.exec();
        Database::closeDatabase();
        if( !isExecuted )
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powiodła się./nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

void CarBlock::on_btnAddInspection_clicked()
{
    emit inProgress();

    if(Database::connectToDatabase("root","Serwis4q@")) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET InspectionDate=:_inspectionDate WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_inspectionDate", ui->dateEditInspection->date());
        bool isExecuted = qry.exec();
        Database::closeDatabase();
        if( !isExecuted )
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powidoła się.\nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

void CarBlock::on_btnRemove_clicked()
{
    emit inProgress();

    if(Database::connectToDatabase("root","Serwis4q@")) {
        if(!isAddBlock) {
            QSqlQuery qry;
            qry.prepare("DELETE FROM car WHERE idCar=:_id");
            qry.bindValue(":_id", idCar);
            bool isExecuted = qry.exec();
            Database::closeDatabase();
            if( !isExecuted )
                QMessageBox::warning(this,"Uwaga!","Usuwanie nie powiodło się.\nERROR: "+qry.lastError().text()+"");
            else {
                QMessageBox::information(this,"Informacja","Usunieto!");
                emit carDeleted(true);
            }
        }

        else {
            QSqlQuery qry;
            qry.prepare("INSERT INTO car(Brand,Model,LicensePlate,InspectionDate,"
                                        "InsuranceDate,Mileage,Status,PhotoPath)"
                         "VALUES(:_Brand,:_Model,:_LicensePlate,:_InspectionDate,"
                         ":_InsuranceDate,:_Mileage,:_Status,:_PhotoPath)");
            if(ui->lblCarName->text().contains(" ")) {
                qry.bindValue(":_Brand", ui->lblCarName->text().split(" ").at(0));
                qry.bindValue(":_Model", ui->lblCarName->text().split(" ").at(1));
            }
            else {
                qry.bindValue(":_Brand", ui->lblCarName->text());
                qry.bindValue(":_Model", ui->lblCarName->text());
            }
            qry.bindValue(":_LicensePlate", ui->lblLicensePlate->text());
            qry.bindValue(":_InspectionDate",ui->dateEditInspection->date());
            qry.bindValue(":_InsuranceDate",ui->dateEditInsurance->date());
            qry.bindValue(":_Status",0);
            qry.bindValue(":_Mileage", ui->lblMileage->text());
            qry.bindValue(":_PhotoPath",addedCarImagePath);
            bool isExecuted = qry.exec();
            Database::closeDatabase();
            if( !isExecuted )
                QMessageBox::warning(this,"Uwaga!","Dodawanie nie powiodło się.\nERROR "+qry.lastError().text()+"");
            else {
                QMessageBox::information(this,"Informacja","Dodano!");
                emit carAdded(false);
            }
        }
    }
    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

void CarBlock::on_btnViewNotes_clicked()
{
    showNotesDialog(-1, idCar);
}

void CarBlock::on_btnAddImage_clicked()
{
    emit inProgress();
    addedCarImagePath = QFileDialog::getOpenFileName(this, tr("Plik z obrazkiem"),
                               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/bez_tytułu.png",
                               tr("Pliki PNG, JPG (*.jpg *.png)"));
    if(addedCarImagePath.isEmpty())
        ui->lblPhoto->setPixmap(QPixmap(":/images/images/car.png"));
    else
        ui->lblPhoto->setPixmap(QPixmap(addedCarImagePath));
    emit progressFinished();
}

QString CarBlock::getCarName()
{
    return ui->lblCarName->text();
}

QString CarBlock::getMileage()
{
    return ui->lblMileage->text();
}

QString CarBlock::getLicensePlate()
{
    return ui->lblLicensePlate->text();
}

QPair<QDate,QDate> CarBlock::getDates()
{
    return qMakePair(ui->dateEditInspection->date(),ui->dateEditInsurance->date());
}
