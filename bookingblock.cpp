#include "bookingblock.h"
#include "ui_bookingblock.h"

extern bool isAdmin;

BookingBlock::BookingBlock(int idB, QString name, QString destination, QString timeBegin, QString timeEnd, bool isTimeVisible, bool isBooking, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookingBlock),
    idBooking(idB)
{
    ui->setupUi(this);
    ui->lblName->setText(name);
    ui->lblBegin->setText(timeBegin);
    ui->lblEnd->setText(timeEnd);
    ui->lblDestination->setText(destination);

    if(isBooking) {
        ui->lblStatus->setStyleSheet("background-color: rgb(255,140,0); border-radius: 5px");
        if(isAdmin)ui->pushButtonDelete->setVisible(true);
        else ui->pushButtonDelete->setVisible(false);
    }
    else {
        ui->lblStatus->setStyleSheet("background-color: rgb(100,149,237); border-radius: 5px");
        ui->pushButtonDelete->setVisible(false);
    }

    ui->lblBegin->setVisible(isTimeVisible);
    ui->lblEnd->setVisible(isTimeVisible);
    ui->lbl->setVisible(isTimeVisible);
}

BookingBlock::~BookingBlock()
{
    delete ui;
}

QString BookingBlock::getName()
{
    return ui->lblName->text();
}

void BookingBlock::on_pushButtonDelete_clicked()
{
    if(Database::connectToDatabase("rezerwacja","rezerwacja")) {
        QSqlQuery qry;
        qry.prepare("DELETE FROM booking WHERE idBooking=:_idBooking");
        qry.bindValue(":_idBooking", idBooking);
        bool isExecuted = qry.exec();
        Database::closeDatabase();
        if( !isExecuted )
            QMessageBox::warning(this,"Uwaga!","Usuwanie nie powiodło się.\nERROR: "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Usunieto!");
            emit refresh();
        }
    }
    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
    }
}
