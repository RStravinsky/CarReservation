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
#include "database.h"
#include "serviceblock.h"
#include "reportdialog.h"
#include "oildialog.h"

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

    explicit CarBlock(bool toAdd = true, int id = 0, QString name = "", QString model = "", QString licensePlate = "", QDate inspectionDate = QDate::currentDate(),
             QDate insuranceDate = QDate::currentDate(), int mileage = 0, Status status = Status::Free, QString photoPath = ":/images/images/car.png", bool isVisible = true, QWidget *parent = 0) ;
    CarBlock(CarBlock &block, QWidget *parent = 0);
    ~CarBlock();
    void setStatus(Status);
    void setBookingTable(QSqlQueryModel * bookTable) {bookingTable = bookTable;}
    void setCarTable(QSqlQueryModel * cTable) {carTable = cTable;}
    void setAdminPermissions(bool isAdmin);
    bool getCarVisible();

public slots:
    void showNotesDialog(int _idNotes, int _idCar);

private slots:
    void on_btnReserve_clicked();
    void on_btnAddInsurance_clicked();
    void on_btnAddInspection_clicked();
    void on_btnRemove_clicked();
    void on_btnViewNotes_clicked();
    void on_btnAddImage_clicked();
    void on_btnAddLicensePlate_clicked();
    void on_btnAddMileage_clicked();
    void on_btnIsVisible_clicked();
    void on_btnViewRepairs_clicked();

    void on_btnPDF_clicked();

    void on_btnOil_clicked();

signals:
    void carDeleted(bool isCopyEnable);
    void carAdded(bool isCopyEnable);
    void inProgress();
    void progressFinished();
    void noteClosed();
    void changeStatusBar(QString,int timeout = 0);

private:
    int * wsk;
    Ui::CarBlock *ui;
    BookingDialog * bookingDialog{nullptr};
    ServiceBlock * serviceBlock{nullptr};
    ReportDialog * reportDialog{nullptr};
    OilDialog * oilDialog{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    QSqlQueryModel * carTable{nullptr};
    int idCar;
    int idNotes;
    QString carNotes;
    bool isAddBlock;
    QString carPhotoPath;
    QString getCarName();
    QString getMileage();
    QString getLicensePlate();
    QPair<QDate,QDate> getDates();
    bool isAdmin;
    QString prevPhotoPath;
    bool isCarVisible;

    void updateImagePath();
    void setVisibleButton(bool isVisible);
    bool showMsgBeforeDelete();

};

#endif // CARBLOCK_H
