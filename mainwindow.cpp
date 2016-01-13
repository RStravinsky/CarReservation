#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CarBlock *c1 = new CarBlock("VW", "Golf", "LU 3456H", "zajęty");
    CarBlock *c2 = new CarBlock("Toyota", "Corolla", "LUB 4353R", "wolny");
    CarBlock *c3 = new CarBlock("Mitsubishi", "Outlander", "LUB 9084W", "zarezerwowany");
    CarBlock *c4 = new CarBlock("Ferrari", "F50", "LUB 7893A", "zarezerwowany");
    CarBlock *c5 = new CarBlock("Lotus", "Eclipse", "LUB 3380B", "zajęty");
    CarBlock *c6 = new CarBlock("Ford", "Mondeo", "LU 0456J", "wolny");

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
    scrollArea ->setGeometry(100,100,640,500);
    scrollArea->setStyleSheet("background-color: #464646;");
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);

}

MainWindow::~MainWindow()
{

    delete ui;
}

