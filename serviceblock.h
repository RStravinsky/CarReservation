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
    static bool isOpen;

private slots:
    void on_lvServices_clicked(const QModelIndex &index);

private:
    Ui::ServiceBlock *ui;
    int idCar;
    QSqlQueryModel * serviceTable{nullptr};
    QSqlQueryModel * listModel{nullptr};
    QStringList nameCompletionList;
    QStringList categoryCompletionList;

    void loadList();
    QStringList nameList;
    QStringListModel nameListModel;

};

#endif // SERVICEBLOCK_H
