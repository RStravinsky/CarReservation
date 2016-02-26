#ifndef DBCONFIGDIALOG_H
#define DBCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class DBConfigDialog;
}

class DBConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBConfigDialog(QWidget *parent = 0);
    ~DBConfigDialog();

private slots:
    void on_cancelButton_clicked();

    void on_runButton_clicked();

private:
    Ui::DBConfigDialog *ui;
};

#endif // DBCONFIGDIALOG_H
