#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    carBlockVector.emplace_back(std::move(new CarBlock("VW", "Golf", "LU 3456H", "zajęty")));
    carBlockVector.emplace_back(std::move(new CarBlock("Toyota", "Corolla", "LUB 4353R", "wolny")));
    carBlockVector.emplace_back(std::move(new CarBlock("Mitsubishi", "Outlander", "LUB 9084W", "zarezerwowany")));
    carBlockVector.emplace_back(std::move(new CarBlock("Ferrari", "F50", "LUB 7893A", "zarezerwowany")));
    carBlockVector.emplace_back(std::move(new CarBlock("Lotus", "Elise", "LUB 3380B", "zajęty")));
    carBlockVector.emplace_back(std::move(new CarBlock("Ford", "Mondeo", "LU 0456J", "wolny")));

    scrollLayout = new QVBoxLayout(ui->scrollArea);
    for(auto pos= carBlockVector.begin();pos!=carBlockVector.end();++pos)
        scrollLayout->addWidget(*pos);
//    scrollLayout->addWidget(c1);
//    scrollLayout->addWidget(c2);
//    scrollLayout->addWidget(c3);
//    scrollLayout->addWidget(c4);
//    scrollLayout->addWidget(c5);
//    scrollLayout->addWidget(c6);

    scrollWidget = new QWidget(ui->centralWidget);
    scrollWidget->setLayout(scrollLayout);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(scrollWidget);

}

MainWindow::~MainWindow()
{

    delete ui;
}
