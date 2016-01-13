#include "carblock.h"
#include "ui_carblock.h"

CarBlock::CarBlock(QString name, QString model, QString licensePlate, QString status, QDate inspectionDate, QDate insuranceDate, int mileage, QString notes, Status status1, QString photoPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarBlock)
{
    ui->setupUi(this);

    ui->lblLicensePlate->setText(licensePlate);
    ui->lblPhoto->setPixmap(QPixmap(photoPath));
    ui->lblCarName->setText(name + QString(" ") + model);
    setStatus(status1);
}

CarBlock::~CarBlock()
{
    delete ui;
}

void CarBlock::setStatus(Status status)
{
    if(status == Status::Free)
        ui->lblStatus->setPixmap(QPixmap(":/images/images/free.png"));
    if(status == Status::Rented)
        ui->lblStatus->setPixmap(QPixmap(":/images/images/rented.png"));
}
