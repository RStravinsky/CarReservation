#include "carblock.h"
#include "ui_carblock.h"

CarBlock::CarBlock(QString name, QString model, QString licensePlate, QString status, QString photoPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarBlock)
{
    ui->setupUi(this);
    ui->lblLicensePlate->setText(licensePlate);
    ui->lblStatus->setText(status);
    ui->lblPhoto->setPixmap(QPixmap::fromImage(QImage(photoPath)));
    ui->groupBox->setTitle(name + QString(" ") + model);
}

CarBlock::~CarBlock()
{
    delete ui;
}
