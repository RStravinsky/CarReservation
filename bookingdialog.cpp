
#include "bookingdialog.h"
#include "ui_bookingdialog.h"

BookingDialog::BookingDialog(QSqlQueryModel *bookTable, QSqlQueryModel *cTable, int id, int vMode,  QWidget *parent) :
    bookingTable(bookTable),
    carTable(cTable),
    idCar(id),
    viewMode(static_cast<ViewMode>(vMode)),
    QDialog(parent),
    ui(new Ui::BookingDialog)
{
    ui->setupUi(this);
    setCalendarColor(ui->calendarWidget,QColor(255,140,0));
    ui->dateTimeEditBegin->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());

    connect(this, SIGNAL(bookedCar()), this, SLOT(updateView()));

    carReservations = new QSqlQueryModel(this);
    statusHistory = new QSqlQueryModel(this);

    QSqlQueryModel * windTitle = new QSqlQueryModel(this);
    windTitle->setQuery(QString("SELECT Brand, Model FROM sigmacars.car WHERE idCar = %1").arg(idCar));
    this->setWindowTitle( QString("Rezerwacja - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,0)).toString() + QString(" ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,1)).toString()
                          );

    scrollWidget = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);

    updateView();

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
    QDate itDate;

    clearCalendarFormat();

    if(viewMode == ViewMode::Booked) {

        format.setBackground(QBrush(QColor(0,0,150,200), Qt::SolidPattern));

        for(int i = 0; i < carReservations->rowCount(); ++i) {

            itDate = carReservations->data(carReservations->index(i,3)).toDate();

            while(itDate <= carReservations->data(carReservations->index(i,4)).toDate()) {
                ui->calendarWidget->setDateTextFormat(itDate, format);
                itDate = itDate.addDays(1);
            }

        }

    }
    else if(viewMode == ViewMode::History) {

        format.setBackground(QBrush(QColor(150,0,0,200), Qt::SolidPattern));

        for(int i = 0; i < statusHistory->rowCount(); ++i) {


            itDate = statusHistory->data(statusHistory->index(i,3)).toDate();

            while(itDate <= statusHistory->data(statusHistory->index(i,4)).toDate()) {
                ui->calendarWidget->setDateTextFormat(itDate, format);
                itDate = itDate.addDays(1);
            }

        }
    }

}

void BookingDialog::loadBlock(int idx)
{
    QDate beginDate;
    QDate endDate;

    if(viewMode == ViewMode::Booked) {

        beginDate = carReservations->data(carReservations->index(idx,3)).toDate();
        endDate = carReservations->data(carReservations->index(idx,4)).toDate();

        if(choosenDate >= beginDate && choosenDate <= endDate) {

            if(beginDate != endDate) {

                if(choosenDate > beginDate && choosenDate < endDate)
                    blockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                               QString("..."), QString("..."), false)));
                else if (choosenDate == beginDate)
                    blockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                               carReservations->data(carReservations->index(idx,3)).toDateTime().time().toString("hh:mm"), QString("..."))));
                else if(choosenDate == endDate)
                    blockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                               QString("..."), carReservations->data(carReservations->index(idx,4)).toDateTime().time().toString("hh:mm"))));
            }
            else
                blockVector.emplace_back(std::move(new BookingBlock(carReservations->data(carReservations->index(idx,1)).toString() + QString(" ") + carReservations->data(carReservations->index(idx,2)).toString(),
                                                                           carReservations->data(carReservations->index(idx,3)).toDateTime().time().toString("hh:mm"), carReservations->data(carReservations->index(idx,4)).toDateTime().time().toString("hh:mm"))));

        }
    }
    else if(viewMode == ViewMode::History) {

        beginDate = statusHistory->data(statusHistory->index(idx,3)).toDate();
        endDate = statusHistory->data(statusHistory->index(idx,4)).toDate();

        if(choosenDate >= beginDate && choosenDate <= endDate) {

            if(beginDate != endDate) {

                if(choosenDate > beginDate && choosenDate < endDate)
                    blockVector.emplace_back(std::move(new BookingBlock(statusHistory->data(statusHistory->index(idx,1)).toString() + QString(" ") + statusHistory->data(statusHistory->index(idx,2)).toString(),
                                                                               QString("..."), QString("..."), false, false)));
                else if (choosenDate == beginDate)
                    blockVector.emplace_back(std::move(new BookingBlock(statusHistory->data(statusHistory->index(idx,1)).toString() + QString(" ") + statusHistory->data(statusHistory->index(idx,2)).toString(),
                                                                               statusHistory->data(statusHistory->index(idx,3)).toDateTime().time().toString("hh:mm"), QString("..."), true, false)));
                else if(choosenDate == endDate)
                    blockVector.emplace_back(std::move(new BookingBlock(statusHistory->data(statusHistory->index(idx,1)).toString() + QString(" ") + statusHistory->data(statusHistory->index(idx,2)).toString(),
                                                                               QString("..."), statusHistory->data(statusHistory->index(idx,4)).toDateTime().time().toString("hh:mm"), true, false)));
            }
            else
                blockVector.emplace_back(std::move(new BookingBlock(statusHistory->data(statusHistory->index(idx,1)).toString() + QString(" ") + statusHistory->data(statusHistory->index(idx,2)).toString(),
                                                                           statusHistory->data(statusHistory->index(idx,3)).toDateTime().time().toString("hh:mm"), statusHistory->data(statusHistory->index(idx,4)).toDateTime().time().toString("hh:mm"), true, false)));

        }

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

    blockVector.clear();
    clearScrollArea();

    if(viewMode == ViewMode::Booked) {
        for(int i = 0; i < carReservations->rowCount(); ++i)
            loadBlock(i);
    }
    else if(viewMode == ViewMode::History) {
        for(int i = 0; i < statusHistory->rowCount(); ++i)
            loadBlock(i);
    }

    for(auto pos = blockVector.begin(); pos != blockVector.end(); ++pos)
        scrollLayout->addWidget(*pos);

    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->setWidgetResizable(true);

    fillCalendar();

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

    QDateTime modelBegin = QDateTime::currentDateTime(), modelEnd = QDateTime::currentDateTime();

    if(ui->dateTimeEditEnd->dateTime() <= ui->dateTimeEditBegin->dateTime()) {
        QMessageBox::warning(this, "Uwaga!", "Data i czas końca rezerwacji musi być większa od początkowej.");
        ui->dateTimeEditBegin->setDateTime(QDateTime::currentDateTime());
        ui->dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());
        return false;
    }


    for(int i = 0; i < bookedDates->rowCount(); ++i) {

        modelBegin = bookedDates->data(bookedDates->index(i,0)).toDateTime();
        modelEnd = bookedDates->data(bookedDates->index(i,1)).toDateTime();

        if(ui->dateTimeEditBegin->dateTime() >= modelBegin && ui->dateTimeEditBegin->dateTime() <= modelEnd) {
            QMessageBox::warning(this, "Uwaga!", "Wybrany termin początkowy jest już zarezerwowany.");
            return false;
        }
        else if(ui->dateTimeEditEnd->dateTime() >= modelBegin && ui->dateTimeEditEnd->dateTime() <= modelEnd) {
            QMessageBox::warning(this, "Uwaga!", "Wybrany termin końcowy jest już zarezerwowany.");
            return false;
        }

    }

    return true;
}

void BookingDialog::on_btnReserve_clicked()
{
    if(isDateFree()) {

        QString name, surname;
        NameDialog n;
        if(!n.getNameAndSurname(name, surname)) return;

        QSqlQuery qry;
        qry.prepare("INSERT INTO booking(Name,Surename,Begin,End, idCar)"
                 "VALUES(:_Name,:_Surename,:_Begin,:_End,:_idCar)");

        qry.bindValue(":_Name", name);
        qry.bindValue(":_Surename", surname);
        qry.bindValue(":_Begin", ui->dateTimeEditBegin->dateTime());
        qry.bindValue(":_End", ui->dateTimeEditEnd->dateTime());
        qry.bindValue(":_idCar",idCar);

        if( !qry.exec() )
            QMessageBox::warning(this,"Informacja","Dodawanie nie powiodło się./nERROR "+qry.lastError().text()+"");
        else {
            QMessageBox::information(this,"Informacja","Dodano!");

        }

        emit bookedCar();
    }

}

void BookingDialog::on_calendarWidget_selectionChanged()
{
    clearScrollArea();
    fillCalendar();
}

void BookingDialog::on_checkBoxBooking_clicked(bool checked)
{
    if(checked) {
        viewMode = ViewMode::Booked;
        ui->checkBoxHistory->setChecked(false);
    }
    else
        viewMode = ViewMode::Nothing;

    clearScrollArea();
    fillCalendar();
}

void BookingDialog::on_checkBoxHistory_clicked(bool checked)
{
    if(checked) {
        viewMode = ViewMode::History;
        ui->checkBoxBooking->setChecked(false);
    }
    else
        viewMode = ViewMode::Nothing;

    clearScrollArea();
    fillCalendar();
}

void BookingDialog::clearCalendarFormat()
{
    QMap<QDate,QTextCharFormat>::iterator itr;
    QTextCharFormat format;
    format.setBackground(QBrush(QColor(255,255,255,255), Qt::SolidPattern));

   for(auto itr : ui->calendarWidget->dateTextFormat().keys())
        ui->calendarWidget->setDateTextFormat(itr, format);

}

void BookingDialog::updateView()
{
    carReservations->setQuery(QString("SELECT * FROM sigmacars.booking WHERE idCar = %1").arg(idCar));
    statusHistory->setQuery(QString("SELECT * FROM sigmacars.history WHERE idCar = %1").arg(idCar));

    clearScrollArea();
    fillCalendar();

    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->setWidgetResizable(true);
}