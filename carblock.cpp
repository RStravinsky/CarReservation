#include "carblock.h"
#include "ui_carblock.h"

CarBlock::CarBlock(QString name, QString model, QString status, QString photoPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarBlock)
{
    ui->setupUi(this);
    ui->lblName->setText(name);
    ui->lblModel->setText(model);
    ui->lblStatus->setText(status);
    ui->lblPhoto->setPixmap(QPixmap::fromImage(QImage(photoPath)));
}

CarBlock::~CarBlock()
{
    delete ui;
}
