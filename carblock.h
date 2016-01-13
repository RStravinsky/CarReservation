#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>

namespace Ui {
class CarBlock;
}

class CarBlock : public QWidget
{
    Q_OBJECT

public:
    explicit CarBlock(QString name, QString model, QString licensePlate, QString status, QDate inspectionDate, QDate insuranceDate, int mileage, QString notes, QWidget *parent = 0);
    ~CarBlock();

private:
    Ui::CarBlock *ui;
};

#endif // CARBLOCK_H
