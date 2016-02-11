#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCompleter>
#include <QtSql>
#include <QStringList>
#include "database.h"

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(QWidget *parent = 0);
    ~NameDialog();
    QString getName() const;
    QString getSurname() const;
    QString getDestination() const;

private slots:
    void on_pushButtonConfirm_released();

private:
    Ui::NameDialog *ui;
    QSqlQueryModel * bookingTable{nullptr};
    QStringList nameList;
    QStringList surnameList;
    QStringList destinationList;
    QCompleter * nameCompleter{nullptr};
    QCompleter * surnameCompleter{nullptr};
    QCompleter * destinationCompleter{nullptr};

    void setCompleterLists();
    void setCompleters();
    QString scrollBarStylesheet();

};

#endif // NAMEDIALOG_H
