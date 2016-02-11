#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
    ui->lineEditName->setFocus();

    bookingTable = new QSqlQueryModel(this);
    bookingTable->setQuery("SELECT Name, Surname, Destination FROM booking;");

    setCompleterLists();
    setCompleters();

}

NameDialog::~NameDialog()
{
    delete ui;
}

QString NameDialog::getName() const
{
    return ui->lineEditName->text();
}

QString NameDialog::getSurname() const
{
    return ui->lineEditSurname->text();
}

QString NameDialog::getDestination() const
{
    return ui->lineEditDestination->text();
}

void NameDialog::on_pushButtonConfirm_released()
{
    QString name = ui->lineEditName->text();
    QString surname = ui->lineEditSurname->text();
    QString destination = ui->lineEditDestination->text();

    if(name.isEmpty() || surname.isEmpty() || destination.isEmpty())
        QMessageBox::warning(this,"Uwaga!","Pole tekstowe nie zostało wypełnione.");

    else this->accept();
}

void NameDialog::setCompleterLists()
{
    for(int i=0; i < bookingTable->rowCount() ;++i) {
        nameList << bookingTable->index(i, 0).data().toString();
        surnameList << bookingTable->index(i, 1).data().toString();
        destinationList << bookingTable->index(i, 2).data().toString();
    }

    nameList.removeDuplicates();
    surnameList.removeDuplicates();
    destinationList.removeDuplicates();
}
void NameDialog::setCompleters()
{
    nameCompleter = new QCompleter(nameList, ui->lineEditName);
    surnameCompleter = new QCompleter(surnameList, ui->lineEditSurname);
    destinationCompleter = new QCompleter(destinationList, ui->lineEditDestination);

    nameCompleter->setCompletionMode(QCompleter::PopupCompletion);
    surnameCompleter->setCompletionMode(QCompleter::PopupCompletion);
    destinationCompleter->setCompletionMode(QCompleter::PopupCompletion);

    nameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    surnameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    destinationCompleter->setCaseSensitivity(Qt::CaseInsensitive);


    nameCompleter->popup()->setStyleSheet(scrollBarStylesheet());
    surnameCompleter->popup()->setStyleSheet(scrollBarStylesheet());
    destinationCompleter->popup()->setStyleSheet(scrollBarStylesheet());

    nameCompleter->setMaxVisibleItems(10);
    surnameCompleter->setMaxVisibleItems(10);
    destinationCompleter->setMaxVisibleItems(10);

    ui->lineEditName->setCompleter(nameCompleter);
    ui->lineEditSurname->setCompleter(surnameCompleter);
    ui->lineEditDestination->setCompleter(destinationCompleter);

}


QString NameDialog::scrollBarStylesheet()
{
    return QString(
                "QScrollBar:vertical {"
                  "border: none;"
                  "background: transparent;"
                  "width: 8px;"
                  "margin: 20px 0px 20px 0px;"
                "}"

                "QScrollBar::handle:vertical {"
                  "background: rgb(107,124,140);"
                  "min-height: 20px;"
                  "border-radius: 2px;"
                "}"

                "QScrollBar::add-line:vertical {"
                   "border: none;"
                   "background: none;"
                "}"

                "QScrollBar::sub-line:vertical {"
                   "border: none;"
                   "background: none;"
                "}"
                );
}
