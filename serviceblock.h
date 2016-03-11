#ifndef SERVICEBLOCK_H
#define SERVICEBLOCK_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QCalendarWidget>
#include <initializer_list>
#include "database.h"


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
    void loadData(const QModelIndex &index);
    void clearWidgets();
    void setCalendarColor(QCalendarWidget *calendarWidget,QColor color);
    bool isDateCorrect();
};

#endif // SERVICEBLOCK_H
