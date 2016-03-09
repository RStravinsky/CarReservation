#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = 0);
    ~ChangePasswordDialog();

private slots:
    void on_pushButtonConfirm_clicked();

private:
    Ui::ChangePasswordDialog *ui;
    QSqlQueryModel * pswQry;
    QString oldPassword;
    bool dataIsEmpty();
};

#endif // CHANGEPASSWORDDIALOG_H
