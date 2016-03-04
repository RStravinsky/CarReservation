#ifndef DBCONFIGDIALOG_H
#define DBCONFIGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QProcess>
#include "mainwindow.h"

namespace Ui {
class DBConfigDialog;
}

class DBConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBConfigDialog(QString line, bool isCreateType, QWidget *parent = 0);
    ~DBConfigDialog();
    static bool readFromFile(QString &line);

signals:
    void connectedToDB(bool);
    void changeStatusBar(QString,int timeout = 0);

private slots:
    void on_runButton_clicked();
    void on_rbRemoteDB_toggled(bool checked);

private:
    Ui::DBConfigDialog *ui;
    bool createMode;
    bool writeToFile(const QString &hostname, int port, const QString &database, const QString &user, const QString &password);
    bool dataIsEmpty();
    void showMsg(const QString &msg);
    void setGrayOut(bool isGray);
};

#endif // DBCONFIGDIALOG_H

