#ifndef DBCONFIGDIALOG_H
#define DBCONFIGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QProcess>

namespace Ui {
class DBConfigDialog;
}

class DBConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBConfigDialog(bool noDB, QWidget *parent = 0);
    ~DBConfigDialog();

signals:
    void connectedToDB(bool);

private slots:
    void on_runButton_clicked();
    void on_rbRemoteDB_toggled(bool checked);

private:
    Ui::DBConfigDialog *ui;
    bool noDataBase;
};

#endif // DBCONFIGDIALOG_H

