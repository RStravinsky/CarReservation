#ifndef NOTESDIALOG_H
#define NOTESDIALOG_H

#include <QDialog>
#include <noteblock.h>
#include <QVBoxLayout>
#include <QtSql>

namespace Ui {
class NotesDialog;
}

class NotesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDialog(int idC, QWidget *parent = 0);
    ~NotesDialog();

public slots:
    void updateView();

private:
    Ui::NotesDialog *ui;
    QVBoxLayout *scrollLayout{nullptr};
    QWidget *scrollWidget{nullptr};
    QSqlQueryModel * notesTable{nullptr};
    std::vector<NoteBlock*> noteBlockVector;
    int idCar;
};

#endif // NOTESDIALOG_H
