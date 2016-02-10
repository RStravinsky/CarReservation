#include "notesdialog.h"
#include "ui_notesdialog.h"

NotesDialog::NotesDialog(int idN, int idC, QWidget *parent) :
    idNotes(idN),
    idCar(idC),
    QDialog(parent),
    ui(new Ui::NotesDialog)
{
    ui->setupUi(this);
    updateView();

    if(idNotes >= 0) selectNote();

}

NotesDialog::~NotesDialog()
{
    delete ui;
}

void NotesDialog::updateView()
{
    const int verticalPosition = ui->scrollArea->verticalScrollBar()->value();

    delete notesTable;
    delete scrollLayout;
    delete scrollWidget;

    notesTable = new QSqlQueryModel(this);
    notesTable->setQuery(QString("SELECT * FROM notes WHERE idCar = %1;").arg(idCar));

    QSqlQueryModel * windTitle = new QSqlQueryModel(this);
    windTitle->setQuery(QString("SELECT Brand, Model, LicensePlate FROM sigmacars.car WHERE idCar = %1").arg(idCar));
    this->setWindowTitle( QString("Uwagi - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,0)).toString() + QString(" ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,1)).toString() + QString(" - ")
                          + windTitle->data(windTitle->index(windTitle->rowCount()-1,2)).toString()
                          );
    delete windTitle;

    noteBlockVector.clear();
    for(int i = 0; i < notesTable->rowCount(); ++i) {
        noteBlockVector.emplace_back(std::move(new NoteBlock(notesTable->data(notesTable->index(i,6)).toInt(), notesTable->data(notesTable->index(i,0)).toInt(),
                                                             notesTable->data(notesTable->index(i,1)).toString(), notesTable->data(notesTable->index(i,2)).toString(),
                                                             notesTable->data(notesTable->index(i,3)).toString(), notesTable->data(notesTable->index(i,4)).toDateTime(),
                                                             notesTable->data(notesTable->index(i,5)).toBool()
                                                            )));


       connect(noteBlockVector[i], SIGNAL(noteDeleted()), this, SLOT(updateView()));
       connect(noteBlockVector[i], SIGNAL(noteDblClicked()), this, SLOT(updateView()));
    }

    noteBlockVector.emplace_back(std::move(new NoteBlock(idCar,0, QString("Wpisz treść uwagi"), QString("Admin"), QString("Admin"), QDateTime::currentDateTime(), false, true)));
    connect(noteBlockVector.back(),SIGNAL(noteAdded()),this,SLOT(updateView()));

    scrollWidget = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);
    for(auto pos= noteBlockVector.begin();pos!=noteBlockVector.end();++pos)
        scrollLayout->addWidget(*pos);
    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->verticalScrollBar()->setValue(verticalPosition);

    emit noteWasRead();
}

void NotesDialog::selectNote()
{
    std::for_each(noteBlockVector.begin(), noteBlockVector.end(), [&] (NoteBlock *& n) {
                                                                                        if(n->getIdNotes() == idNotes) {
                                                                                            n->setSelection();
                                                                                            ui->scrollArea->verticalScrollBar()->setValue(n->y());
                                                                                        }
                                                                                      });

}
