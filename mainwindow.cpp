#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CarBlock *c1 = new CarBlock("VW", "Golf", "zajęty");
    CarBlock *c2 = new CarBlock("Toyota", "Corolla", "wolny");
    CarBlock *c3 = new CarBlock("Mitsubishi", "Outlander", "zarezerwowany");
    CarBlock *c4 = new CarBlock("Ferrari", "F50", "zarezerwowany");
    CarBlock *c5 = new CarBlock("Lotus", "Eclipse", "zajęty");
    CarBlock *c6 = new CarBlock("Ford", "Mondeo", "wolny");

    scrollLayout = new QVBoxLayout(ui->centralWidget);
    scrollLayout->addWidget(c1);
    scrollLayout->addWidget(c2);
    scrollLayout->addWidget(c3);
    scrollLayout->addWidget(c4);
    scrollLayout->addWidget(c5);
    scrollLayout->addWidget(c6);

    scrollWidget = new QWidget(ui->centralWidget);
    scrollWidget->setLayout(scrollLayout);

    scrollArea = new QScrollArea(ui->centralWidget);
    scrollArea ->setGeometry(0,0,700,300);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);

}

MainWindow::~MainWindow()
{

    delete ui;
}
