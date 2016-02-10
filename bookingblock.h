#ifndef BOOKINGBLOCK_H
#define BOOKINGBLOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTime>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class BookingBlock;
}

class BookingBlock : public QWidget
{
    Q_OBJECT

public:
    explicit BookingBlock(int idB, QString name, QString destination, QString timeBegin, QString timeEnd, bool isTimeVisible = true, bool isBooking = true, QWidget *parent = 0);
    ~BookingBlock();
    QString getName();

private slots:
    void on_pushButtonDelete_clicked();

signals:
    void refresh();

private:
    Ui::BookingBlock *ui;
    int idBooking;
};

#endif // BOOKINGBLOCK_H
