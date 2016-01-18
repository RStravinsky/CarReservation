#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <vector>
#include <bookingblock.h>
#include <QtSql>
#include <QTextCharFormat>
#include <QDebug>

namespace Ui {
class BookingDialog;
}

class BookingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookingDialog(QSqlQueryModel *bookTable, QSqlQueryModel *cTable, int id, QWidget *parent = 0);
    ~BookingDialog();

private slots:
    void dateClicked(QDate date);

    void on_calendarWidget_clicked(const QDate &date);

    void on_btnReserve_clicked();

private:
    Ui::BookingDialog *ui;
    QVBoxLayout *scrollLayout;
    QWidget *scrollWidget;
    std::vector<BookingBlock*> bookingBlockVector;
    QDate choosenDate;
    QSqlQueryModel * bookingTable {nullptr};
    QSqlQueryModel * carTable {nullptr};
    QSqlQueryModel * carReservations {nullptr};
    int idCar;


    void fillCalendar();
    void loadBookingBlock(int idx);
    void clearScrollArea();
    bool isDateFree();

    void setCalendarColor(QCalendarWidget *&calendarWidget,QColor color);
};

#endif // BOOKINGDIALOG_H
