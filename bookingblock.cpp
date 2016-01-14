#include "bookingblock.h"
#include "ui_bookingblock.h"

BookingBlock::BookingBlock(QString name, QString status, QTime begin, QTime end, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookingBlock)
{
    ui->setupUi(this);
    ui->lblName->setText(name);

}

BookingBlock::~BookingBlock()
{
    delete ui;
}
