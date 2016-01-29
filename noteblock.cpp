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
    ui->lblIsRead->installEventFilter(this);

    if(!isAdded) {
        ui->txtEditContents->setText(contents);
        ui->txtEditContents->setReadOnly(true);
        (name == "Admin" && surename == "Admin") ? ui->lblNameSurename->setText(QString(name)) : ui->lblNameSurename->setText(QString("%1 %2").arg(name).arg(surename));
        ui->lblDate->setText(dateTime.toString("dd-MM-yyyy hh:mm:ss"));

        if(isRead) {
            ui->lblIsRead->setPixmap(QPixmap(":/images/images/read.png"));
        }
        else {
            ui->lblIsRead->setPixmap(QPixmap(":/images/images/new.png"));
        }
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

bool NoteBlock::eventFilter(QObject *obj, QEvent *event)
{

    if(obj == ui->lblIsRead) {

        if (event->type() == QEvent::MouseButtonDblClick) {

            QSqlQuery qry;
            qry.prepare("UPDATE notes SET isRead=1 WHERE idNotes = :_idNotes;");
            qry.bindValue(":_idNotes", idNotes);

            if(!qry.exec()) {
                return false;
            }
            else {
                emit noteDblClicked();
                return true;;
            }
        }

    }

    return false;
}

void NoteBlock::setSelection()
{
    ui->txtEditContents->setStyleSheet("background-color: rgba(146, 202, 128, 131); color: black");
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

        qry.prepare("INSERT INTO notes(Contents,Name,Surename,Datetime,isRead,idCar)"
                     "VALUES(:_Contents,:_Name,:_Surename,:_Datetime,:_isRead,:_idCar)"
                    );

        qry.bindValue(":_Contents", ui->txtEditContents->toPlainText());
        qry.bindValue(":_Name", ui->lblNameSurename->text());
        qry.bindValue(":_Surename", ui->lblNameSurename->text());
        qry.bindValue(":_Datetime", dateOfAdd);
        qry.bindValue(":_isRead", 1);
        qry.bindValue(":_idCar", idCar);


        if( !qry.exec() )
            QMessageBox::warning(this,"Informacja","Dodawanie nie powiodło się./nERROR "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Dodano!");
            emit noteAdded();
        }
    }
}
