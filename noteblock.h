#ifndef NOTEBLOCK_H
#define NOTEBLOCK_H

#include <QWidget>
#include <QDateTime>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class NoteBlock;
}

class NoteBlock : public QWidget
{
    Q_OBJECT

public:
    explicit NoteBlock(int idC, int idN, QString contents, QString name, QString surename, QDateTime dateTime, bool toAdd = false, QWidget *parent = 0);
    ~NoteBlock();

private:
    Ui::NoteBlock *ui;
    int idCar;
    int idNotes;
    bool isAdded;

signals:
    void noteDeleted();
    void noteAdded();

private slots:
    void on_btnRemove_clicked();
};

#endif // NOTEBLOCK_H
