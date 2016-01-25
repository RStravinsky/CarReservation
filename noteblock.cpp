#include "noteblock.h"
#include "ui_noteblock.h"

NoteBlock::NoteBlock(int idC, int idN, QString contents, QString name, QString surename, QDateTime dateTime, bool isNoteRead, bool toAdd, QWidget *parent) :
    idCar(idC),
    idNotes(idN),
    QWidget(parent),
    isRead(isNoteRead),
    isAdded(toAdd),
    ui(new Ui::NoteBlock)
{
    ui->setupUi(this);

    if(!isAdded) {
        ui->txtEditContents->setText(contents);
        ui->txtEditContents->setReadOnly(true);
        (name == "Admin" && surename == "Admin") ? ui->lblNameSurename->setText(QString(name)) : ui->lblNameSurename->setText(QString("%1 %2").arg(name).arg(surename));
        ui->lblDate->setText(dateTime.toString("dd-MM-yyyy hh:mm:ss"));

        if(isRead) ui->lblIsRead->setPixmap(QPixmap(":/images/images/read.png"));
        else ui->lblIsRead->setPixmap(QPixmap(":/images/images/new.png"));
    }
    else {
        ui->txtEditContents->setText(contents);
        ui->txtEditContents->setReadOnly(false);
        ui->lblNameSurename->setText(surename);
        ui->lblDate->setText(" - ");
        ui->btnRemove->setIcon(QIcon(":/images/images/add.png"));
        ui->txtEditContents->setFocus();
    }

}

NoteBlock::~NoteBlock()
{
    delete ui;
}


void NoteBlock::on_btnRemove_clicked()
{

    if(!isAdded) {
        QSqlQuery qry;
        qry.prepare("DELETE FROM notes WHERE idNotes=:_id");
        qry.bindValue(":_id", idNotes);
        if( !qry.exec() )
            QMessageBox::warning(this,"Informacja","Usuwanie nie powiodła się./nERROR: "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Usunieto!");
            emit noteDeleted();
        }
    }
    else {
        QSqlQuery qry;
        QDateTime dateOfAdd = QDateTime::currentDateTime();
        ui->lblDate->setText(dateOfAdd.toString());

        qry.prepare("INSERT INTO notes(Contents,Name,Surename,Datetime, idCar)"
                     "VALUES(:_Contents,:_Name,:_Surename,:_Datetime,:_idCar)"
                    );

        qry.bindValue(":_Contents", ui->txtEditContents->toPlainText());
        qry.bindValue(":_Name", ui->lblNameSurename->text());
        qry.bindValue(":_Surename", ui->lblNameSurename->text());
        qry.bindValue(":_Datetime", dateOfAdd);
        qry.bindValue(":_idCar", idCar);
        if( !qry.exec() )
            QMessageBox::warning(this,"Informacja","Dodawanie nie powiodło się./nERROR "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Dodano!");
            emit noteAdded();
        }
    }


}
