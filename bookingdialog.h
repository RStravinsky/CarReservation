#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <vector>
#include <bookingblock.h>
#include <namedialog.h>
#include <QtSql>
#include <QTextCharFormat>
#include <QDebug>
#include <QMessageBox>
#include <QMenu>
#include <QMap>

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
    void dateClicked(QDate date);

    void on_calendarWidget_clicked(const QDate &date);

    void on_btnReserve_clicked();

    void on_calendarWidget_selectionChanged();

    void on_checkBoxBooking_clicked(bool checked);

    void on_checkBoxHistory_clicked(bool checked);

    void updateView();

signals:
    void bookedCar();

private:
    Ui::BookingDialog *ui;
    QVBoxLayout *scrollLayout;
    QWidget *scrollWidget;
    std::vector<BookingBlock*> blockVector;
    QDate choosenDate;
    QSqlQueryModel * bookingTable {nullptr};
    QSqlQueryModel * carTable {nullptr};
    QSqlQueryModel * statusHistory {nullptr};
    QSqlQueryModel * carReservations {nullptr};
    int idCar;

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

    void setCalendarColor(QCalendarWidget *&calendarWidget,QColor color);
};

#endif // BOOKINGDIALOG_H