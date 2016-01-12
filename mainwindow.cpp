#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    layout = new QVBoxLayout;

    CarBlock *c1 = new CarBlock("VW", "GOLF", "zarezerwowany");
    CarBlock *c2 = new CarBlock("Toyota", "Corolla", "wolny");
    CarBlock *c3 = new CarBlock("Mitsubishi", "Outlander", "zarezerwowany");
    CarBlock *c4 = new CarBlock("Ferrari", "F50", "zarezerwowany");
    CarBlock *c5 = new CarBlock("Lotus", "Eclipse", "zarezerwowany");
    CarBlock *c6 = new CarBlock("Morda", "Mordak", "zarezerwowany");

    layout->addWidget(c1, 0 , 0);
    layout->addWidget(c2, 1 , 0);
    layout->addWidget(c3, 2 , 0);
    layout->addWidget(c4, 3 , 0);
    layout->addWidget(c5, 4 , 0);

    layout->addWidget(c6, 5 , 0);
//    layout->addWidget(c2, 6 , 0);
//    layout->addWidget(c3, 7 , 0);
//    layout->addWidget(c4, 8 , 0);
//    layout->addWidget(c5, 9 , 0);

    ui->scrollArea->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}
