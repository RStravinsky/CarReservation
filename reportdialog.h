#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include <QTextCursor>
#include <QTextTable>
#include <QPrinter>
#include <QtPrintSupport>
#include <QDesktopServices>
#include <QFileDialog>
#include <QtSql>
#include <QCalendarWidget>

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(int idC, QWidget *parent = 0);
    ~ReportDialog();

private slots:
    void on_cancelButton_clicked();

    void on_exportButton_clicked();

private:
    Ui::ReportDialog *ui;
    int idCar;
    QSqlQueryModel * historyModel{nullptr};
    QSqlQueryModel * reportTitle{nullptr};
    enum TabFormat
    {
        Header = 0,
        Table = 1
    };

    QTextTableFormat tableFormat( TabFormat format);
    void addHeaderToDocument(QTextDocument *document, QTextCursor *cursor);
    void populateTable(QTextCursor *cursor);
    void populatePDF();
    void setCalendarColor(QCalendarWidget *calendarWidget,QColor color);
};

#endif // REPORTDIALOG_H
