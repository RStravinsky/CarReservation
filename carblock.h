#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>
#include <bookingdialog.h>
#include <notesdialog.h>
#include <memory>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>
#include <QPair>
#include <QRegExp>

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

    explicit CarBlock(int id, QString name, QString model, QString licensePlate, QDate inspectionDate, QDate insuranceDate, int mileage, Status status = CarBlock::Free, QString photoPath = ":/images/images/car.png",bool toAdd = false, QWidget *parent = 0);
    CarBlock(CarBlock & block,QWidget *parent = 0);
    ~CarBlock();
    void setStatus(Status);
    void setBookingTable(QSqlQueryModel * bookTable) {bookingTable = bookTable;}
    void setCarTable(QSqlQueryModel * cTable) {carTable = cTable;}
    void setAdminPermissions(bool isAdmin);

public slots:
    void showNotesDialog(int _idNotes, int _idCar);

private slots:
    void on_btnReserve_clicked();
    void on_btnAddInsurance_clicked();
    void on_btnAddInspection_clicked();
    void on_btnRemove_clicked();
    void on_btnViewNotes_clicked();
    void on_btnAddImage_clicked();

signals:
    void carDeleted(bool isCopyEnable);
    void carAdded(bool isCopyEnable);
    void inProgress();
    void progressFinished();
    void noteClosed();

private:
    Ui::CarBlock *ui;
    BookingDialog * bookingDialog{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    QSqlQueryModel * carTable{nullptr};
    int idCar;
    int idNotes;
    QString carNotes;
    bool isAddBlock;
    QString addedCarImagePath;
    QString getCarName();
    QString getMileage();
    QString getLicensePlate();
    QPair<QDate,QDate> getDates();
    bool isAdmin;
};

#endif // CARBLOCK_H
