#include "bookingblock.h"
#include "ui_bookingblock.h"

BookingBlock::BookingBlock(QString name, QString timeBegin, QString timeEnd, bool isTimeVisible, bool isBooking, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookingBlock)
{
    ui->setupUi(this);
    ui->lblName->setText(name);
    ui->lblBegin->setText(timeBegin);
    ui->lblEnd->setText(timeEnd);

    if(isBooking)
        ui->lblStatus->setStyleSheet("background-color: rgb(0,0,150,200); border-radius: 5px");
    else
        ui->lblStatus->setStyleSheet("background-color: rgb(150,0,0,200); border-radius: 5px");

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
