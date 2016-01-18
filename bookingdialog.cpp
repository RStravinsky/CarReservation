#include "bookingdialog.h"
#include "ui_bookingdialog.h"

BookingDialog::BookingDialog(QSqlQueryModel *bookTable, QSqlQueryModel *cTable, int id, QWidget *parent) :
    bookingTable(bookTable),
    carTable(cTable),
    idCar(id),
    QDialog(parent),
    ui(new Ui::BookingDialog)
{
    ui->setupUi(this);
    setCalendarColor(ui->calendarWidget,QColor(255,140,0));

    carReservations = new QSqlQueryModel(this);
    carReservations->setQuery(QString("SELECT * FROM sigmacars.booking WHERE idCar = %1").arg(idCar));

    QSqlQueryModel * windTitle = new QSqlQueryModel(this);
    windTitle->setQuery(QString("SELECT Brand, Model FROM sigmacars.car WHERE idCar = %1").arg(idCar));
    this->setWindowTitle( QString("Rezerwacja - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,0)).toString() + QString(" ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,1)).toString()
                          );

    fillCalendar();

    scrollWidget = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);

}

BookingDialog::~BookingDialog()
{
    delete ui;
}

void BookingDialog::dateClicked(QDate date)
{
    choosenDate = date;
}

void BookingDialog::fillCalendar()
{
    QTextCharFormat format;
    format.setBackground(QBrush(QColor(150,0,0,200), Qt::SolidPattern));

    for(int i = 0; i < bookingTable->rowCount(); ++i) {

        if(bookingTable->data(bookingTable->index(i,5)).toInt() == idCar) {

            QDate itDate = bookingTable->data(bookingTable->index(i,3)).toDate();

            while(itDate <= bookingTable->data(bookingTable->index(i,4)).toDate()) {
                ui->calendarWidget->setDateTextFormat(itDate, format);
                itDate = itDate.addDays(1);
            }

        }

    }

}

void BookingDialog::loadBookingBlock(int idx)
{

    QDate beginDate = carReservations->data(carReservations->index(idx,3)).toDate();
    QDate endDate = carReservations->data(carReservations->index(idx,4)).toDate();

    //qDebug() << "beginDate: " << beginDate << ", endDate: " << endDate << ", choosenDate: " << choosenDate;
    if(choosenDate >= beginDate && choosenDate <= endDate) {

        if(beginDate != endDate) {

            if(choosenDate > beginDate && choosenDate < endDate)
                bookingBlockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                           QString("..."), QString("..."), false)));
            else if (choosenDate == beginDate)
                bookingBlockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                           carReservations->data(carReservations->index(idx,3)).toDateTime().time().toString("hh:mm"), QString("..."))));
            else if(choosenDate == endDate)
                bookingBlockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                           QString("..."), carReservations->data(carReservations->index(idx,4)).toDateTime().time().toString("hh:mm"))));
        }
        else
            bookingBlockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                       carReservations->data(carReservations->index(idx,3)).toDateTime().time().toString("hh:mm"), carReservations->data(carReservations->index(idx,4)).toDateTime().time().toString("hh:mm"))));
    }
}

void BookingDialog::setCalendarColor(QCalendarWidget *&calendarWidget,QColor color)
{
    QWidget *calendarNavBar = calendarWidget->findChild<QWidget *>("qt_calendar_navigationbar");
    if (calendarNavBar) {
        QPalette pal = calendarNavBar->palette();
        pal.setColor(calendarNavBar->backgroundRole(), color);
        calendarNavBar->setPalette(pal);
    }
}

void BookingDialog::on_calendarWidget_clicked(const QDate &date)
{
    choosenDate = date;

    bookingBlockVector.clear();
    clearScrollArea();

    for(int i = 0; i < carReservations->rowCount(); ++i)
        loadBookingBlock(i);

    for(auto pos = bookingBlockVector.begin(); pos != bookingBlockVector.end(); ++pos)
        scrollLayout->addWidget(*pos);

    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->setWidgetResizable(true);

}

void BookingDialog::clearScrollArea()
{
    QLayoutItem *child;
    while ((child =  scrollLayout->takeAt(0)) != 0) {
        delete child->widget();
    }

}

bool BookingDialog::isDateFree()
{
    QSqlQueryModel * bookedDates = new QSqlQueryModel(this);
    bookedDates->setQuery(QString("SELECT Begin, End FROM sigmacars.booking WHERE idCar = %1").arg(idCar));

    QDateTime begin;
    begin.setDate(choosenDate);
    begin.setTime(ui->timeEditBegin->time());
    QDateTime end;
    end.setDate(choosenDate);
    end.setTime(ui->timeEditEnd->time());

    for(int i = 0; i < bookedDates->rowCount(); ++i) {
        // HERE LAST

    }
}

void BookingDialog::on_btnReserve_clicked()
{

}
