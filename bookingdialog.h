#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <vector>
#include <bookingblock.h>

namespace Ui {
class BookingDialog;
}

class BookingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookingDialog(QWidget *parent = 0);
    ~BookingDialog();

private slots:
    void dateClicked(QDate date);

private:
    Ui::BookingDialog *ui;
    QVBoxLayout *scrollLayout;
    QWidget *scrollWidget;
    std::vector<BookingBlock*> bookingBlockVector;
    QDate choosenDate;

    void setCalendarColor(QCalendarWidget *&calendarWidget,QColor color);
};

#endif // BOOKINGDIALOG_H
