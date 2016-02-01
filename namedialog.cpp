#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
}

NameDialog::~NameDialog()
{
    delete ui;
}

QString NameDialog::getName()
{
    return ui->lineEditName->text();
}

QString NameDialog::getSurname()
{
    return ui->lineEditSurname->text();
}

void NameDialog::on_pushButtonConfirm_released()
{
    QString name = ui->lineEditName->text();
    QString surname = ui->lineEditSurname->text();

    if(name.isEmpty() || surname.isEmpty())
        QMessageBox::information(this,"Informacja","Pole tekstowe nie zostało wypełnione.");

    else this->accept();
}
