#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>
#include <bookingdialog.h>
#include <memory>
#include <QDebug>
#include <QtSql>

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

    explicit CarBlock(int id, QString name, QString model, QString licensePlate, QDate inspectionDate, QDate insuranceDate, int mileage, QString notes, Status status = CarBlock::Free, QString photoPath = ":/images/images/car.png",QWidget *parent = 0);
    ~CarBlock();
    void setStatus(Status);
    void setBookingTable(QSqlQueryModel * bookTable) {bookingTable = bookTable;}
    void setCarTable(QSqlQueryModel * cTable) {carTable = cTable;}

private slots:
    void on_btnReserve_clicked();

private:
    Ui::CarBlock *ui;
    BookingDialog * bookingDialog{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    QSqlQueryModel * carTable{nullptr};
    int idCar;
};

#endif // CARBLOCK_H
