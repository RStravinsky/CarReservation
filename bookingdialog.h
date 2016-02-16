#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QtSql>
#include <QTextCharFormat>
#include <QDebug>
#include <QMessageBox>
#include <QMenu>
#include <QTimer>
#include <QScrollBar>
#include <vector>
#include "bookingblock.h"
#include "namedialog.h"
#include "database.h"

namespace Ui {
class BookingDialog;
}

class BookingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookingDialog(QSqlQueryModel *bookTable, QSqlQueryModel *cTable, int id, int vMode = ViewMode::Booked, QWidget *parent = 0);
    ~BookingDialog();

private slots:
    void on_calendarWidget_clicked(const QDate &date);
    void on_btnReserve_clicked();
    void updateView();
    void onTimerOverflow();
    void on_btnShowHistory_clicked();
    void on_btnShowReservation_clicked();
    void on_pushButton_clicked();

signals:
    void bookedCar();

private:
    Ui::BookingDialog *ui;
    QVBoxLayout *scrollLayout{nullptr};
    QWidget *scrollWidget{nullptr};
    std::vector<BookingBlock*> blockVector;
    QDate choosenDate;
    QSqlQueryModel * bookingTable {nullptr};
    QSqlQueryModel * carTable {nullptr};
    QSqlQueryModel * statusHistory {nullptr};
    QSqlQueryModel * carReservations {nullptr};
    int idCar;
    QTimer *timer;
    bool firstInit{true};

    enum ViewMode {
        Booked,
        History,
        Nothing
    };

    ViewMode viewMode;

    void fillCalendar();
    void loadBlock(int idx);
    void clearScrollArea();
    bool isDateFree();
    void clearCalendarFormat();
    void setCalendarColor(QCalendarWidget *calendarWidget, QColor color);
};

#endif // BOOKINGDIALOG_H
