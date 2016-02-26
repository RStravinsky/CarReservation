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
        (name == "Admin" && surename == "Admin") ? ui->lblNameSurname->setText(QString(name)) : ui->lblNameSurname->setText(QString("%1 %2").arg(name).arg(surename));
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
        ui->lblNameSurname->setText(surename);
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

            if(Database::connectToDatabase()) {
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
            else  QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
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
    if(Database::connectToDatabase()) {
        if(!isAdded) {

            if(!showMsgBeforeDelete())
                    return;

            QSqlQuery qry;
            qry.prepare("DELETE FROM notes WHERE idNotes=:_id");
            qry.bindValue(":_id", idNotes);
            if(!qry.exec())
                QMessageBox::warning(this,"Uwaga!","Usuwanie nie powiodła się.\nERROR: "+qry.lastError().text()+"");
            else {
                QMessageBox::information(this,"Informacja","Usunieto!");
                emit noteDeleted();
            }
        }
        else {
            QSqlQuery qry;
            QDateTime dateOfAdd = QDateTime::currentDateTime();
            ui->lblDate->setText(dateOfAdd.toString());

            qry.prepare("INSERT INTO notes(Contents,Name,Surname,Datetime,isRead,idCar)"
                         "VALUES(:_Contents,:_Name,:_Surname,:_Datetime,:_isRead,:_idCar)"
                        );

            qry.bindValue(":_Contents", ui->txtEditContents->toPlainText());
            qry.bindValue(":_Name", ui->lblNameSurname->text());
            qry.bindValue(":_Surname", ui->lblNameSurname->text());
            qry.bindValue(":_Datetime", dateOfAdd);
            qry.bindValue(":_isRead", 1);
            qry.bindValue(":_idCar", idCar);

            if(!qry.exec())
                QMessageBox::warning(this,"Uwaga!","Dodawanie nie powiodło się.\nERROR "+qry.lastError().text()+"");
            else {
                QMessageBox::information(this,"Informacja","Dodano!");
                emit noteAdded();
            }
        }
    }
    else QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
}

bool NoteBlock::showMsgBeforeDelete()
{
    QMessageBox msgBox(QMessageBox::Question, tr("Usuwanie!"), tr("<font face=""Calibri"" size=""3"" color=""gray"">Czy na pewno chcesz usunąć wiadomość?</font>"), QMessageBox::Yes | QMessageBox::No );

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
