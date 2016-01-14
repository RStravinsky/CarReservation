#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>
#include <bookingdialog.h>
#include <memory>

namespace Ui {
class CarBlock;
}

class CarBlock : public QWidget
{
    Q_OBJECT

public:
    enum Status {
        Free,
        Rented,
    };

    explicit CarBlock(QString name, QString model, QString licensePlate, QString status, QDate inspectionDate, QDate insuranceDate, int mileage, QString notes, Status status1 = CarBlock::Free, QString photoPath = ":/images/images/car.png",QWidget *parent = 0);
    ~CarBlock();
    void setStatus(Status);

private slots:
    void on_btnReserve_clicked();

private:
    Ui::CarBlock *ui;
    BookingDialog * bookingDialog{nullptr};
};

#endif // CARBLOCK_H
