#include "serviceblock.h"
#include "ui_serviceblock.h"

ServiceBlock::ServiceBlock(int idC, QWidget *parent) :
    QDialog(parent),
    idCar(idC),
    ui(new Ui::ServiceBlock)
{
    ui->setupUi(this);

    setCalendarColor(ui->deBegin->calendarWidget(),QColor(255,140,0));
    setCalendarColor(ui->deEnd->calendarWidget(),QColor(255,140,0));
    setCalendarColor(ui->deEvent->calendarWidget(),QColor(255,140,0));
    setCalendarColor(ui->deGuarantee->calendarWidget(),QColor(255,140,0));

    QSqlQueryModel * windTitle = new QSqlQueryModel(this);
    windTitle->setQuery(QString("SELECT Brand, Model, LicensePlate FROM car WHERE idCar = %1").arg(idCar));
    this->setWindowTitle( QString("Naprawy - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,0)).toString() + QString(" ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,1)).toString() + QString(" - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,2)).toString()
                          );

    connect(this,SIGNAL(saved()),this,SLOT(updateView()),Qt::DirectConnection);
    connect(this,SIGNAL(deleted()),this,SLOT(updateView()));

    // fill category combobox
    QStringList categoryList({QString("Silnik"),QString("Zawieszenie"),QString("Elektronika"),
                             QString("Elektyka"),QString("Układ napędowy"),QString("Układ hamulcowy"),
                             QString("Układ kierowniczy"),QString("Nadwozie")});
    ui->cbCategory->addItems(categoryList);

    updateView();

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
    if(Database::isOpen()) {
        serviceTable = new QSqlQueryModel(this);
        serviceTable->setQuery(QString("SELECT * FROM service WHERE idCar = %1 ORDER BY EventDate DESC;").arg(idCar));
        loadData(index);
    }
    else QMessageBox::critical(this,tr("Błąd!"), tr("Utracono połączenie z bazą danych!"));
}

void ServiceBlock::setCalendarColor(QCalendarWidget *calendarWidget,QColor color)
{
    QWidget *calendarNavBar = calendarWidget->findChild<QWidget *>("qt_calendar_navigationbar");
    if (calendarNavBar) {
        QPalette pal = calendarNavBar->palette();
        pal.setColor(calendarNavBar->backgroundRole(), color);
        calendarNavBar->setPalette(pal);
    }
}

bool ServiceBlock::isDateCorrect()
{
    if(ui->deBegin->date() > ui->deEnd->date()) {
        QMessageBox::warning(this, tr("Uwaga!"), tr("Nie poprawny czas trwania naprawy."));
        return false;
    }

    if(ui->deEvent->date() > ui->deBegin->date()) {
        QMessageBox::warning(this, tr("Uwaga!"), tr("Data zdarzenia jest większa od początkowej daty naprawy."));
        return false;
    }

    return true;
}

void ServiceBlock::loadData(const QModelIndex &index)
{
    if(ui->lvServices->currentIndex().data(Qt::DisplayRole ).toString() == "Dodaj naprawę ...")
        clearWidgets();
    else {
        ui->pbDelete->setVisible(true);
        idService = serviceTable->index(index.row()-1,0).data().toInt();
        ui->leName->setText( serviceTable->index(index.row()-1,1).data().toString() );
        ui->cbCategory->setCurrentText( serviceTable->index(index.row()-1,2).data().toString() );
        ui->deEvent->setDate( serviceTable->index(index.row()-1,3).data().toDate() );
        ui->deGuarantee->setDate( serviceTable->index(index.row()-1,4).data().toDate() );
        ui->deBegin->setDate( serviceTable->index(index.row()-1,5).data().toDate() );
        ui->deEnd->setDate( serviceTable->index(index.row()-1,6).data().toDate() );
        ui->leCost->setText( serviceTable->index(index.row()-1,7).data().toString() );
        ui->teNotes->setText( serviceTable->index(index.row()-1,8).data().toString() );
    }
}

void ServiceBlock::clearWidgets()
{
    ui->pbDelete->setVisible(false);
    ui->leName->clear();
    ui->cbCategory->setCurrentIndex(0);
    ui->deEvent->setDate(QDate::currentDate());
    ui->deGuarantee->setDate(QDate::currentDate());
    ui->deBegin->setDate(QDate::currentDate());
    ui->deEnd->setDate(QDate::currentDate());
    ui->leCost->clear();
    ui->teNotes->clear();
}

void ServiceBlock::updateView()
{
    if(Database::isOpen()) {
        nameList.clear();
        ui->lvServices->clear();

        serviceTable = new QSqlQueryModel(this);
        serviceTable->setQuery(QString("SELECT * FROM service WHERE idCar = %1 ORDER BY EventDate DESC;").arg(idCar));

        // fill repairs list
        nameList << "Dodaj naprawę ...";
        for(int i=0; i<serviceTable->rowCount(); ++i)
            nameList << serviceTable->data(serviceTable->index(i,1)).toString();
        for(auto elem: nameList)
            ui->lvServices->addItem(elem);
        ui->lvServices->item(0)->setSelected(true);
        ui->lvServices->setFocus();
        ui->pbDelete->setVisible(false);
    }

    else QMessageBox::critical(this,tr("Błąd!"), tr("Utracono połączenie z bazą danych!"));
}

void ServiceBlock::on_pbSave_clicked()
{
     if(Database::isOpen()) {
         // insert new repair
         if(!isDateCorrect())
             return;

         if(ui->lvServices->currentIndex().data(Qt::DisplayRole ).toString() == "Dodaj naprawę ...") {
             QSqlQuery qry;
             qry.prepare("INSERT INTO service(Name,ComponentCategory,EventDate,GuaranteeDate,"
                                         "BeginDate,EndDate,Cost,Notes,idCar)"
                          "VALUES(:_Name,:_ComponentCategory,:_EventDate,:_GuaranteeDate,"
                          ":_BeginDate,:_EndDate,:_Cost,:_Notes,:_idCar)");

             if(ui->leName->text()!="") qry.bindValue(":_Name", ui->leName->text());
             qry.bindValue(":_ComponentCategory", ui->cbCategory->currentText());
             qry.bindValue(":_EventDate", ui->deEvent->date());
             qry.bindValue(":_GuaranteeDate", ui->deGuarantee->date());
             qry.bindValue(":_BeginDate", ui->deBegin->date());
             qry.bindValue(":_EndDate", ui->deEnd->date());
             qry.bindValue(":_Cost", ui->leCost->text() );
             qry.bindValue(":_Notes", ui->teNotes->toPlainText());
             qry.bindValue(":_idCar", idCar);
             if(!qry.exec())
                 QMessageBox::warning(this,tr("Uwaga!"),"Dodawanie nie powiodło się.\nERROR: "+qry.lastError().text()+"");
             else {
                 QMessageBox::information(this,tr("Informacja"),tr("Dodano naprawę!"));
                 ui->lvServices->addItem(ui->leName->text());
                 clearWidgets();
                 ui->lvServices->item(0)->setSelected(true);
             }
         }

         // update repair
         else {
             QSqlQuery qry;
             qry.prepare("UPDATE service SET Name=:_Name,ComponentCategory=:_ComponentCategory,"
                         "EventDate=:_EventDate,GuaranteeDate=:_GuaranteeDate,"
                         "BeginDate=:_BeginDate,EndDate=:_EndDate,Cost=:_Cost,Notes=:_Notes WHERE idService=:_idS");
             if(ui->leName->text()!="") qry.bindValue(":_Name", ui->leName->text());
             qry.bindValue(":_ComponentCategory", ui->cbCategory->currentText());
             qry.bindValue(":_EventDate", ui->deEvent->date());
             qry.bindValue(":_GuaranteeDate", ui->deGuarantee->date());
             qry.bindValue(":_BeginDate", ui->deBegin->date());
             qry.bindValue(":_EndDate", ui->deEnd->date());
             qry.bindValue(":_Cost", ui->leCost->text() );
             qry.bindValue(":_Notes", ui->teNotes->toPlainText());
             qry.bindValue(":_idS", idService);
             if( !qry.exec() )
                 QMessageBox::warning(this,tr("Uwaga!"),"Aktualizacja nie powiodła się.\nERROR: "+qry.lastError().text()+"");
             else {
                 QMessageBox::information(this,tr("Informacja"),tr("Zaktualizowano!"));
                 clearWidgets();
                 emit saved();
             }
         }
     }
     else QMessageBox::critical(this,tr("Błąd!"), tr("Utracono połączenie z bazą danych!"));
}

void ServiceBlock::on_pbDelete_clicked()
{
    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("DELETE FROM service WHERE idService=:_idS");
        qry.bindValue(":_idS", idService);
        if( !qry.exec() )
            QMessageBox::warning(this,tr("Uwaga!"),"Usuwanie nie powiodło się.\nERROR: "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,tr("Informacja"),tr("Usunięto naprawę!"));
            clearWidgets();
            emit deleted();
        }
    }
    else QMessageBox::critical(this,tr("Błąd!"), tr("Utracono połączenie z bazą danych!"));
}

