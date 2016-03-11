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
    begin = QTime::fromString(timeBegin,"hh:mm");
    ui->lblEnd->setText(timeEnd);
    ui->lblDestination->setText(destination);

    if(isBooking) {
        ui->lblStatus->setStyleSheet("background-color: rgb(0,186,18); border-radius: 5px");
        if(isAdmin)ui->pushButtonDelete->setVisible(true);
        else ui->pushButtonDelete->setVisible(false);
    }
    else {
        ui->lblStatus->setStyleSheet("background-color: rgb(80,90,210); border-radius: 5px");
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
    emit inProgress();
    if(Database::isOpen()) {

        if(!showMsgBeforeDelete())
                return;

        QSqlQuery qry;
        qry.prepare("DELETE FROM booking WHERE idBooking=:_idBooking");
        qry.bindValue(":_idBooking", idBooking);
        if(!qry.exec() )
            QMessageBox::warning(this,tr("Uwaga!"),"Usuwanie nie powiodło się.\nERROR: "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,tr("Informacja"),tr("Usunięto rezerwację!"));
            emit refresh();
        }
    }
    else QMessageBox::critical(this,tr("Błąd!"), tr("Utracono połączenie z bazą danych!"));

    emit progressFinished();
}

bool BookingBlock::showMsgBeforeDelete()
{
    QMessageBox msgBox(QMessageBox::Question, tr("Usuwanie!"), tr("<font face=""Calibri"" size=""3"" color=""gray"">Czy na pewno chcesz usunąć rezerwację?</font>"), QMessageBox::Yes | QMessageBox::No );

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
                         "font-size: 14;"
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
