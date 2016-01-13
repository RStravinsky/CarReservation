#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    carBlockVector.emplace_back(std::move(new CarBlock("VW", "Golf", "LU 3456H", CarBlock::Free,":/images/images/corolla.jpg")));
    carBlockVector.emplace_back(std::move(new CarBlock("Toyota", "Corolla", "LUB 4353R", CarBlock::Rented,":/images/images/lancer.jpg")));
    carBlockVector.emplace_back(std::move(new CarBlock("Mitsubishi", "Outlander", "LUB 9084W", CarBlock::Free)));
    carBlockVector.emplace_back(std::move(new CarBlock("Ferrari", "F50", "LUB 7893A", CarBlock::Rented)));
    carBlockVector.emplace_back(std::move(new CarBlock("Lotus", "Elise", "LUB 3380B", CarBlock::Rented)));
    carBlockVector.emplace_back(std::move(new CarBlock("Ford", "Mondeo", "LU 0456J", CarBlock::Free)));

    scrollWidget = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);
    for(auto pos= carBlockVector.begin();pos!=carBlockVector.end();++pos) {
        scrollLayout->addWidget(*pos);
        scrollLayout->setMargin(0);
    }
    ui->scrollArea->setWidget(scrollWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
