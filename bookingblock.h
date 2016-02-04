#ifndef BOOKINGBLOCK_H
#define BOOKINGBLOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTime>

namespace Ui {
class BookingBlock;
}

class BookingBlock : public QWidget
{
    Q_OBJECT

public:
    explicit BookingBlock(QString name, QString timeBegin, QString timeEnd, bool isTimeVisible = true, bool isBooking = true, QWidget *parent = 0);
    ~BookingBlock();
    QString getName();

private:
    Ui::BookingBlock *ui;
};

#endif // BOOKINGBLOCK_H
