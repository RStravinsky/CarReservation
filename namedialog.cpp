#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonConfirm, SIGNAL(released()), this, SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
}

NameDialog::~NameDialog()
{
    delete ui;
}

bool NameDialog::getNameAndSurname(QString &name, QString &surname)
{
    if(this->exec()) {
        name = ui->lineEditName->text();
        surname = ui->lineEditSurname->text();
        return true;
    }
    else {
        name = "";
        surname = "";
        return false;
    }
}
