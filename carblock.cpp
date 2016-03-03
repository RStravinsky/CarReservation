#include "carblock.h"
#include "ui_carblock.h"

extern std::shared_ptr<NotesDialog> notesDialogPointer;

CarBlock::CarBlock( bool toAdd,int id, QString name, QString model, QString licensePlate, QDate inspectionDate, QDate insuranceDate, int mileage, Status status,
                   QString photoPath, bool isVisible, QWidget *parent):
    QWidget(parent),
    ui(new Ui::CarBlock),
    idCar(id),
    isAddBlock(toAdd)
{
    ui->setupUi(this);

    if(!isAddBlock){
        ui->lblLicensePlate->setText(licensePlate);
        ui->lblCarName->setText(name + QString(" ") + model);
        ui->lblMileage->setText(QString::number(mileage));
        setVisibleButton(isVisible);
    }

    else if(isAddBlock){
        ui->btnReserve->setVisible(false);
        ui->btnAddInspection->setVisible(false);
        ui->btnAddInsurance->setVisible(false);
        ui->btnAddLicensePlate->setVisible(false);
        ui->btnAddMileage->setVisible(false);
        ui->lblStatus->setVisible(false);
        ui->btnViewNotes->setVisible(false);
        ui->btnOil->setVisible(false);
        ui->btnAddLicensePlate->setVisible(false);
        ui->btnAddMileage->setVisible(false);
        ui->lblPersonImage->setVisible(false);
        ui->btnViewRepairs->setVisible(false);
        ui->lblMileage->setReadOnly(false);
        ui->lblCarName->setReadOnly(false);
        ui->lblLicensePlate->setReadOnly(false);
        ui->btnRemove->setIcon(QIcon(":/images/images/add.png"));
        ui->btnPDF->setVisible(false);
        QRegExp rx("^\\w+\\s\\w+$");
        ui->lblCarName->setValidator(new QRegExpValidator(rx, this));
        setVisibleButton(isVisible);
    }

    ui->lblPhoto->setPixmap(QPixmap(photoPath));
    carPhotoPath = photoPath;
    ui->dateEditInspection->setDate(inspectionDate);
    ui->dateEditInsurance->setDate(insuranceDate);

    QDate tmp;
    tmp = QDate::currentDate().addDays(7);

    if(tmp >= inspectionDate)
        ui->lblInspectionImage->setPixmap(QPixmap(":/images/images/inspection_warning.png"));
    else ui->lblInspectionImage->setPixmap(QPixmap(":/images/images/inspection.png"));

    if(tmp >= insuranceDate)
        ui->lblInsuranceImage->setPixmap(QPixmap(":/images/images/insurance_warning.png"));
    else ui->lblInsuranceImage->setPixmap(QPixmap(":/images/images/insurance.png"));

    setStatus(status);
    if(status && isVisible) {
        QSqlQueryModel * historyTable = new QSqlQueryModel(this);
        historyTable->setQuery(QString("SELECT Name, Surname, Destination FROM history WHERE idCar = %1 AND End IS NULL").arg(idCar));

        ui->lblPerson->setText(
                    historyTable->data(historyTable->index(historyTable->rowCount()-1,0)).toString() + QString(" ") +
                    historyTable->data(historyTable->index(historyTable->rowCount()-1,1)).toString() + QString(" ") +
                    historyTable->data(historyTable->index(historyTable->rowCount()-1,2)).toString()
                    );

        delete historyTable;
    }
    else {
        ui->lblPersonImage->setVisible(false);
        ui->lblPerson->setVisible(false);
    }

    if(!isVisible)
        ui->btnReserve->setVisible(false);
}

CarBlock::CarBlock(CarBlock &block, QWidget *parent):
    QWidget(parent),
    ui(new Ui::CarBlock)
{
    ui->setupUi(this);

    isAddBlock = true;

    ui->btnReserve->setVisible(false);
    ui->btnAddInspection->setVisible(false);
    ui->btnAddInsurance->setVisible(false);
    ui->lblStatus->setVisible(false);
    ui->btnViewNotes->setVisible(false);
    ui->btnAddLicensePlate->setVisible(false);
    ui->btnAddMileage->setVisible(false);
    ui->btnViewRepairs->setVisible(false);
    ui->lblMileage->setReadOnly(false);
    ui->lblCarName->setReadOnly(false);
    ui->lblPersonImage->setVisible(false);
    ui->lblLicensePlate->setReadOnly(false);
    ui->btnOil->setVisible(false);
    ui->btnPDF->setVisible(false);

    ui->lblPhoto->setPixmap(block.carPhotoPath);
    carPhotoPath = block.carPhotoPath;
    ui->btnRemove->setIcon(QIcon(":/images/images/add.png"));
    setVisibleButton(block.getCarVisible());
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
        ui->btnOil->setVisible(false);
        ui->dateEditInspection->setVisible(false);
        ui->dateEditInsurance->setVisible(false);
        ui->btnAddInspection->setVisible(false);
        ui->btnAddInsurance->setVisible(false);
        ui->btnAddLicensePlate->setVisible(false);
        ui->btnAddMileage->setVisible(false);
        ui->btnViewNotes->setVisible(false);
        ui->btnRemove->setVisible(false);
        ui->btnAddImage->setVisible(false);
        ui->btnIsVisible->setVisible(false);
        ui->btnViewRepairs->setVisible(false);
        ui->btnPDF->setVisible(false);
        ui->lblCarName->setGeometry(10,0,650,60);
    }
    else {
        ui->lblMileage->setReadOnly(false);
        ui->lblLicensePlate->setReadOnly(false);
        ui->lblCarName->setGeometry(60,0,650,60);
    }
}

bool CarBlock::getCarVisible()
{
    return isCarVisible;
}

void CarBlock::showNotesDialog(int _idNotes, int _idCar)
{
    if(_idCar == idCar) {
        if(Database::isOpen()) {
            emit inProgress();
                notesDialogPointer = std::shared_ptr<NotesDialog>(new NotesDialog(_idNotes, _idCar));
                if(notesDialogPointer.use_count() == 1 && notesDialogPointer.get()->exec() == NotesDialog::Rejected){
                    if(!(BookingDialog::isOpen || ServiceBlock::isOpen)) emit progressFinished();
                    emit noteClosed();
                }
        }
        else {
            QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        }
    }
}

void CarBlock::on_btnReserve_clicked()
{
    if(Database::isOpen()) {
        bookingDialog = new BookingDialog(idCar);
        emit inProgress();
        if(bookingDialog->exec()== BookingDialog::Rejected)
            emit progressFinished();
        delete bookingDialog;
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }
}

void CarBlock::on_btnAddMileage_clicked()
{
    emit inProgress();

    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET Mileage=:_Mileage WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_Mileage", ui->lblMileage->text());
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powiodła się.\nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

void CarBlock::on_btnAddLicensePlate_clicked()
{
    emit inProgress();

    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET LicensePlate=:_LicensePlate WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_LicensePlate", ui->lblLicensePlate->text());
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powiodła się.\nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

void CarBlock::on_btnAddInsurance_clicked()
{
    emit inProgress();

    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET InsuranceDate=:_insuranceDate WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_insuranceDate", ui->dateEditInsurance->date());
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powiodła się.\nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

void CarBlock::on_btnAddInspection_clicked()
{
    emit inProgress();

    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET InspectionDate=:_inspectionDate WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_inspectionDate", ui->dateEditInspection->date());
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powidoła się.\nERROR: "+qry.lastError().text()+"");
        else
            QMessageBox::information(this,"Informacja","Zaktualizowano!");
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

void CarBlock::on_btnRemove_clicked()
{
    emit inProgress();

    if(Database::isOpen()) {
        if(!isAddBlock) {

            if(!showMsgBeforeDelete()){
                    emit progressFinished();
                    return;
            }

            QSqlQuery qry;
            qry.prepare("DELETE FROM car WHERE idCar=:_id");
            qry.bindValue(":_id", idCar);
            if(!qry.exec())
                QMessageBox::warning(this,"Uwaga!","Usuwanie nie powiodło się.\nERROR: "+qry.lastError().text()+"");
            else {
                QMessageBox::information(this,"Informacja","Usunieto!");
                emit carDeleted(true);
            }
        }

        else {
            QSqlQuery qry;
            qry.prepare("INSERT INTO car(Brand,Model,LicensePlate,InspectionDate,"
                                        "InsuranceDate,Mileage,Status,PhotoPath,IsVisible)"
                         "VALUES(:_Brand,:_Model,:_LicensePlate,:_InspectionDate,"
                         ":_InsuranceDate,:_Mileage,:_Status,:_PhotoPath,:_IsVisible)");
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
            if(getCarVisible())qry.bindValue(":_Status",0);
            else qry.bindValue(":_Status",1);
            qry.bindValue(":_Mileage", ui->lblMileage->text());
            if(!carPhotoPath.isEmpty())qry.bindValue(":_PhotoPath",carPhotoPath);
            else qry.bindValue(":_PhotoPath", QString(":/images/images/car.png"));
            qry.bindValue(":_IsVisible", getCarVisible());
            if(!qry.exec())
                QMessageBox::warning(this,"Uwaga!","Dodawanie nie powiodło się.\nERROR "+qry.lastError().text()+"");
            else {
                QMessageBox::information(this,"Informacja","Dodano!");
                emit carAdded(false);
            }
        }
    }
    else {
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
    QString choosenPhotoPath = QFileDialog::getOpenFileName(this, tr("Plik z obrazkiem"),
                               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                               tr("Pliki PNG, JPG (*.jpg *.png)"));

    if(choosenPhotoPath.isEmpty())
        ui->lblPhoto->setPixmap(QPixmap(carPhotoPath));
    else {
        ui->lblPhoto->setPixmap(QPixmap(choosenPhotoPath));
        carPhotoPath = choosenPhotoPath;
        if(!isAddBlock)
            updateImagePath();
    }
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

void CarBlock::updateImagePath()
{
    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("UPDATE car SET PhotoPath=:_PhotoPath WHERE idCar=:_id");
        qry.bindValue(":_id", idCar);
        qry.bindValue(":_PhotoPath", carPhotoPath);
        if(!qry.exec())
            QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powidoła się.\nERROR: "+qry.lastError().text()+"");
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }
}

void CarBlock::setVisibleButton(bool isVisible)
{
    isCarVisible = isVisible;
    if(isVisible)
        ui->btnIsVisible->setIcon(QIcon(":/images/images/visible.png"));
    else
        ui->btnIsVisible->setIcon(QIcon(":/images/images/notvisible.png"));
}

bool CarBlock::showMsgBeforeDelete()
{
    QMessageBox msgBox(QMessageBox::Question, tr("Usuwanie!"), tr("<font face=""Calibri"" size=""3"" color=""gray"">Czy na pewno chcesz usunać samochód?</font>"), QMessageBox::Yes | QMessageBox::No );

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
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Nie"));
    if (msgBox.exec() == QMessageBox::No)
        return false;

    return true;
}

void CarBlock::on_btnIsVisible_clicked()
{
    static bool isVisible = isCarVisible;
    if(!isAddBlock) {
        if(Database::isOpen()) {
            isVisible = !isVisible;
            setVisibleButton(isVisible);
            QSqlQuery qry;
            qry.prepare("UPDATE car SET IsVisible=:_IsVisible,Status=:_Status WHERE idCar=:_id");
            qry.bindValue(":_id", idCar);
            qry.bindValue(":_IsVisible", isVisible);
            qry.bindValue(":_Status", !isVisible);
            if(!qry.exec())
                QMessageBox::warning(this,"Uwaga!","Aktualizacja nie powidoła się.\nERROR: "+qry.lastError().text()+"");
            else
                emit carDeleted(true); // only for update
        }
        else {
            QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
            emit changeStatusBar("Nie można połączyć z bazą danych");
        }
    }
    else {
        isVisible = !isVisible;
        setVisibleButton(isVisible);
    }
}

void CarBlock::on_btnViewRepairs_clicked()
{
    if(Database::isOpen()) {
        serviceBlock = new ServiceBlock(idCar);
        emit inProgress();
        if(serviceBlock->exec()== ServiceBlock::Rejected)
            emit progressFinished();
        delete serviceBlock;
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }
}

void CarBlock::on_btnPDF_clicked()
{
    if(Database::isOpen()) {
        reportDialog = new ReportDialog(idCar);
        emit inProgress();
        if(reportDialog->exec()== ReportDialog::Rejected)
            emit progressFinished();
        delete reportDialog;
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }
}

void CarBlock::on_btnOil_clicked()
{
    if(Database::isOpen()) {
        oilDialog = new OilDialog(idCar);
        emit inProgress();
        if(oilDialog->exec()== OilDialog::Rejected)
            emit progressFinished();
        delete oilDialog;
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }
}
