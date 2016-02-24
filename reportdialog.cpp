#include "reportdialog.h"
#include "ui_reportdialog.h"

ReportDialog::ReportDialog(int idC, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog),
    idCar(idC)
{
    ui->setupUi(this);

    historyModel = new QSqlQueryModel(this);
    reportTitle = new QSqlQueryModel(this);

    QDate initDate;
    initDate.setDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);

    ui->beginDateTimeEdit->setDate(initDate);
    ui->beginDateTimeEdit->setTime(QTime::currentTime());
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    setCalendarColor(ui->beginDateTimeEdit->calendarWidget(),QColor(255,140,0));
    setCalendarColor(ui->endDateTimeEdit->calendarWidget(),QColor(255,140,0));
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

QTextTableFormat ReportDialog::tableFormat(TabFormat format)
{
    QTextTableFormat tableFormat;
    if(format == TabFormat::Table) {

        tableFormat.setAlignment(Qt::AlignCenter);
        tableFormat.setCellSpacing(0);
        tableFormat.setCellPadding(3);
        tableFormat.setTopMargin(10);
        tableFormat.setBottomMargin(10);
        tableFormat.setBorderBrush(Qt::black);

        QVector<QTextLength> widths;
        widths << QTextLength(QTextLength::PercentageLength, 4) << QTextLength(QTextLength::PercentageLength, 14) << QTextLength(QTextLength::PercentageLength, 15)
               << QTextLength(QTextLength::PercentageLength, 16) << QTextLength(QTextLength::PercentageLength, 16) << QTextLength(QTextLength::PercentageLength, 12)
               << QTextLength(QTextLength::PercentageLength, 18) << QTextLength(QTextLength::PercentageLength, 4);

        tableFormat.setColumnWidthConstraints(widths);


    }

    if(format == TabFormat::Header) {

        tableFormat.setAlignment(Qt::AlignCenter);
        tableFormat.setCellSpacing(0);
        tableFormat.setCellPadding(3);
        tableFormat.setTopMargin(10);
        tableFormat.setBottomMargin(0);
        tableFormat.setBorder(0);


        QVector<QTextLength> widths;
        widths << QTextLength(QTextLength::PercentageLength, 10) << QTextLength(QTextLength::PercentageLength, 45) << QTextLength(QTextLength::PercentageLength, 45);

        tableFormat.setColumnWidthConstraints(widths);
    }

    return tableFormat;
}

void ReportDialog::addHeaderToDocument(QTextDocument *document,QTextCursor *cursor)
{
    QImage logoImage(":/images/images/sigma.png");
    document->addResource(QTextDocument::ImageResource, QUrl(":/images/images/sigma.png"), logoImage);

    QTextTable *headerTable = cursor->insertTable(1, 3, tableFormat(TabFormat::Header));
    //headerTable->mergeCells(1,0,1,3);

    QTextBlockFormat blockFormat;
    QTextImageFormat logoFormat;
    logoFormat.setName(":/images/images/sigma.png");
    logoFormat.setWidth(40);

    headerTable->cellAt(0,0).firstCursorPosition();

    blockFormat.setAlignment(Qt::AlignBottom);
    cursor->insertBlock(blockFormat);
    cursor->insertImage(logoFormat);

    cursor->movePosition(QTextCursor::NextCell);
    blockFormat.setAlignment(Qt::AlignRight);
    cursor->insertBlock(blockFormat);
    QTextCharFormat charFormat;
    charFormat.setFont(QFont("Calibri", 8, QFont::Bold));

    reportTitle->setQuery(QString("SELECT Brand, Model, LicensePlate FROM car WHERE idCar = %1").arg(idCar));

    cursor->insertText(QString("Raport wypożyczeń - ")
                       + reportTitle->data(reportTitle->index(reportTitle->rowCount()-1,0)).toString() + QString(" ")
                       + reportTitle->data(reportTitle->index(reportTitle->rowCount()-1,1)).toString() + QString(" - ")
                       + reportTitle->data(reportTitle->index(reportTitle->rowCount()-1,2)).toString() , charFormat);

    QString headerText = "Wygenerowano: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");

    cursor->movePosition(QTextCursor::NextCell);
    blockFormat.setAlignment(Qt::AlignRight);
    cursor->insertBlock(blockFormat);
    QTextCharFormat charFormat2;
    charFormat2.setFont(QFont("Calibri", 8));
    cursor->insertText(headerText, charFormat2);

}

void ReportDialog::populateTable(QTextCursor *cursor)
{
    QTextDocument tableDocument;
    QTextCursor tableDocumentCursor(&tableDocument);

    QTextCursor tableCursor;
    QTextBlockFormat blockFormat;
    cursor->insertHtml("<hr>");

    historyModel->setQuery(QString("SELECT idHistory, Name, Surname, Begin, End, Destination, Target, Distance FROM history WHERE idCar = %1 AND "
                                   "End >= \"%2\" AND End <= \"%3\" ORDER BY End ASC;").arg(idCar).arg(ui->beginDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->endDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                            );

    QTextTable *table = tableDocumentCursor.insertTable(historyModel->rowCount() + 1, historyModel->columnCount(), tableFormat(TabFormat::Table));

    QStringList headerList ({"L.p.","Imię", "Nazwisko", "Od", "Do", "Lokalizacja", "Cel", "Km"});

    for(int i = 1; i < historyModel->rowCount() + 1; ++i) {
        tableCursor = table->cellAt(i,0).firstCursorPosition();
        blockFormat.setAlignment(Qt::AlignHCenter);
        tableCursor.insertBlock(blockFormat);
        tableCursor.insertText(QString::number(i));
    }

     for(int j = 0; j < historyModel->columnCount(); ++j) {
         tableCursor = table->cellAt(0,j).firstCursorPosition();
         blockFormat.setAlignment(Qt::AlignHCenter);
         tableCursor.insertBlock(blockFormat);
         tableCursor.insertText(headerList.at(j));
     }

    for(int i = 1; i < historyModel->rowCount() + 1; ++i) {

        for(int j = 1; j < historyModel->columnCount(); ++j) {

            tableCursor = table->cellAt(i,j).firstCursorPosition();

            if(j == 3 || j == 4) {
                blockFormat.setAlignment(Qt::AlignHCenter);
                tableCursor.insertBlock(blockFormat);
                tableCursor.insertText(historyModel->index(i-1, j).data().toString().replace("T"," "));
            }
            else {
                blockFormat.setAlignment(Qt::AlignHCenter);
                tableCursor.insertBlock(blockFormat);
                tableCursor.insertText(historyModel->index(i-1, j).data().toString());
            }
        }
    }

    cursor->insertFragment(QTextDocumentFragment(&tableDocument));
}

void ReportDialog::populatePDF()
{
    QTextDocument document;
    document.setDefaultFont(QFont("Calibri", 9));
    QTextCursor textCursor(&document);

    addHeaderToDocument(&document,&textCursor);
    textCursor.movePosition(QTextCursor::NextRow);
    textCursor.movePosition(QTextCursor::NextBlock);
    populateTable(&textCursor);

    QString filters( "Pliki PDF (*.pdf);;Wszystkie pliki (*.*)" );
    QString pdfFilter( "Pliki PDF (*.pdf)" );
    QString pdfFile;
    pdfFile = QFileDialog::getSaveFileName( this, "Wyeksportuj do pliku", QDir::homePath(), filters, &pdfFilter );
    if(pdfFile.isEmpty()) return;
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(pdfFile);
    printer.setOutputFormat(QPrinter::PdfFormat);
    document.print(&printer);
    QMessageBox::information( this, "Informacja", "Wyeksportowano do pliku:\n" + pdfFile );
    QDesktopServices::openUrl(pdfFile);

}

void ReportDialog::on_cancelButton_clicked()
{
    this->reject();
}

void ReportDialog::on_exportButton_clicked()
{
    if(Database::connectToDatabase("rezerwacja","rezerwacja"))
        populatePDF();
    else {
        Database::closeDatabase();
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
    }
}

void ReportDialog::setCalendarColor(QCalendarWidget *calendarWidget,QColor color)
{
    QWidget *calendarNavBar = calendarWidget->findChild<QWidget *>("qt_calendar_navigationbar");
    if (calendarNavBar) {
        QPalette pal = calendarNavBar->palette();
        pal.setColor(calendarNavBar->backgroundRole(), color);
        calendarNavBar->setPalette(pal);
    }
}
