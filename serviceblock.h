#ifndef SERVICEBLOCK_H
#define SERVICEBLOCK_H

#include <QDialog>
#include "database.h"
#include <QtSql>
#include <initializer_list>
#include <QMessageBox>
#include <QVector>
#include <QListWidgetItem>

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
    void on_pbSave_clicked();
    void on_pbDelete_clicked();
    void updateView();

signals:
    void saved();
    void deleted();

private:
    Ui::ServiceBlock *ui;
    int idCar;
    int idService;
    QSqlQueryModel * serviceTable{nullptr};
    QSqlQueryModel * listModel{nullptr};
    QStringList nameCompletionList;
    QStringList categoryCompletionList;

    void loadList();
    QStringList nameList;
    QStringList idList;
    QVector<QListWidgetItem*> lwiVector;
    void loadData();
    void clearWidgets();
};

#endif // SERVICEBLOCK_H
