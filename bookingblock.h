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
    QTime getTime() const {
        return begin;
    }

private slots:
    void on_pushButtonDelete_clicked();

signals:
    void refresh();
    void inProgress();
    void progressFinished();

private:
    Ui::BookingBlock *ui;
    int idBooking;
    bool showMsgBeforeDelete();
    QTime begin;
};

#endif // BOOKINGBLOCK_H
