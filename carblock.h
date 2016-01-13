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
    explicit CarBlock(QString name, QString model, QString licensePlate, QString status, QString photoPath = ":/images/images/car.png", QWidget *parent = 0);
    ~CarBlock();

private:
    Ui::CarBlock *ui;
};

#endif // CARBLOCK_H
