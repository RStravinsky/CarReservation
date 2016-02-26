#ifndef NOTEBLOCK_H
#define NOTEBLOCK_H

#include <QWidget>
#include <QDateTime>
#include <QtSql>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class NoteBlock;
}

class NoteBlock : public QWidget
{
    Q_OBJECT

public:
    explicit NoteBlock(int idC, int idN, QString contents, QString name, QString surename, QDateTime dateTime, bool isNoteRead, bool toAdd = false, QWidget *parent = 0);
    ~NoteBlock();
    void setSelection();
    int getIdNotes() {return idNotes;}

private:
    Ui::NoteBlock *ui;
    int idCar;
    int idNotes;
    bool isAdded;
    bool isRead;
    bool eventFilter(QObject *obj, QEvent *event);
    bool showMsgBeforeDelete();

signals:
    void noteDeleted();
    void noteAdded();
    void noteDblClicked();

private slots:
    void on_btnRemove_clicked();
};

#endif // NOTEBLOCK_H
