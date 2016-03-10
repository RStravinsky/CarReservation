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
    static QString currentAddress;
    static QString user;
    static QString password;

signals:
    void connectedToDB(bool);
    void changeStatusBar(QString);

private slots:
    void on_runButton_clicked();
    void on_rbRemoteDB_toggled(bool checked);
    void on_deleteButton_clicked();
    void on_cbImport_toggled(bool checked);
    void on_pathButton_clicked();

private:
    Ui::DBConfigDialog *ui;
    QString importPath;
    bool createMode;
    bool writeToFile(const QString &hostname, int port, const QString &database, const QString &user, const QString &password);
    bool dataIsEmpty();
    void showMsg(const QString &msg);
    void setGrayOut(bool isGray);
    bool createDatabase(QString command);
    bool showMsgBeforeDelete();
};

#endif // DBCONFIGDIALOG_H

