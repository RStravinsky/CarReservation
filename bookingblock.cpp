#include "bookingblock.h"
#include "ui_bookingblock.h"

BookingBlock::BookingBlock(QString name, QString timeBegin, QString timeEnd, bool isTimeVisible, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookingBlock)
{
    ui->setupUi(this);
    ui->lblName->setText(name);
    ui->lblBegin->setText(timeBegin);
    ui->lblEnd->setText(timeEnd);

    ui->lblBegin->setVisible(isTimeVisible);
    ui->lblEnd->setVisible(isTimeVisible);
    ui->lbl->setVisible(isTimeVisible);

}

BookingBlock::~BookingBlock()
{
    delete ui;
}

QString BookingBlock::getName()
{
    return ui->lblName->text();
}
