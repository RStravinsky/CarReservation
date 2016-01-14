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
    explicit BookingBlock(QString name,QString status, QTime begin, QTime end, QWidget *parent = 0);
    ~BookingBlock();

private:
    Ui::BookingBlock *ui;

};

#endif // BOOKINGBLOCK_H
