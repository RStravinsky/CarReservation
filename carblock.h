#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>
#include <bookingdialog.h>
#include <memory>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>

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

    explicit CarBlock(int id, QString name, QString model, QString licensePlate, QDate inspectionDate, QDate insuranceDate, int mileage, QString notes, Status status = CarBlock::Free, QString photoPath = ":/images/images/car.png",bool toAdd = false, QWidget *parent = 0);
    ~CarBlock();
    void setStatus(Status);
    void setBookingTable(QSqlQueryModel * bookTable) {bookingTable = bookTable;}
    void setAdminPermissions(bool isAdmin);

private slots:
    void on_btnReserve_clicked();
    void on_btnAddInsurance_clicked();
    void on_btnAddInspection_clicked();
    void on_btnRemove_clicked();
    void on_btnViewNotes_clicked();
    void on_btnAddImage_clicked();

signals:
    void carDeleted();
    void carAdded();

private:
    Ui::CarBlock *ui;
    int idCar;
    BookingDialog * bookingDialog{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    QString carNotes;
    bool isAddBlock;
    QString addedCarImagePath;
};

#endif // CARBLOCK_H
