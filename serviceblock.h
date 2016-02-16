#ifndef SERVICEBLOCK_H
#define SERVICEBLOCK_H

#include <QDialog>
#include "database.h"
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class ServiceBlock;
}

class ServiceBlock : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceBlock(int idC, QWidget *parent = 0);
    ~ServiceBlock();

private slots:
    void on_lvServices_clicked(const QModelIndex &index);

private:
    Ui::ServiceBlock *ui;
    int idCar;
    QSqlQueryModel * serviceTable{nullptr};
    QSqlQueryModel * listModel{nullptr};

};

#endif // SERVICEBLOCK_H
