#ifndef OILDIALOG_H
#define OILDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QString>
#include "database.h"

namespace Ui {
class OilDialog;
}

class OilDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OilDialog(int id, QWidget *parent = 0);
    ~OilDialog();

private slots:
    void on_btnAddOil_clicked();

private:
    Ui::OilDialog *ui;
    int idCar;
};

#endif // OILDIALOG_H
