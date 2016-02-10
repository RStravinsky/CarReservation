#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
    ui->lineEditName->setFocus();
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
