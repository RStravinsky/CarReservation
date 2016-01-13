#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>
#include <QDebug>

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

    explicit CarBlock(QString name, QString model, QString licensePlate, QDate inspectionDate, QDate insuranceDate, int mileage, QString notes, Status status = CarBlock::Free, QString photoPath = ":/images/images/car.png",QWidget *parent = 0);
    ~CarBlock();
    void setStatus(Status);

private:
    Ui::CarBlock *ui;
};

#endif // CARBLOCK_H
