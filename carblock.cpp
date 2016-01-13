#include "carblock.h"
#include "ui_carblock.h"

CarBlock::CarBlock(QString name, QString model, QString licensePlate, Status status, QString photoPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarBlock)
{
    ui->setupUi(this);
    ui->lblLicensePlate->setText(licensePlate);
    ui->lblPhoto->setPixmap(QPixmap::fromImage(QImage(photoPath)));
    ui->lblCarName->setText(name + QString(" ") + model);
    setStatus(status);
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
