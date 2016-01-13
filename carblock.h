#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>

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

    explicit CarBlock(QString name, QString model, QString licensePlate, Status status, QString photoPath = ":/images/images/car.png", QWidget *parent = 0);
    ~CarBlock();

    void setStatus(Status);


private:
    Ui::CarBlock *ui;
};

#endif // CARBLOCK_H
