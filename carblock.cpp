#include "carblock.h"
#include "ui_carblock.h"

CarBlock::CarBlock(QString name, QString model, QString licensePlate, QString status, QDate inspectionDate, QDate insuranceDate, int mileage, QString notes, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarBlock)
{
    ui->setupUi(this);
    ui->lblLicensePlate->setText(licensePlate);
    ui->lblStatus->setText(status);
    ui->lblPhoto->setPixmap(QPixmap::fromImage(QImage(":/images/images/car.png")));
    ui->groupBox->setTitle(name + QString(" ") + model);
    ui->lblMileage->setText(QString::number(mileage) + " km");

}

CarBlock::~CarBlock()
{
    delete ui;
}
