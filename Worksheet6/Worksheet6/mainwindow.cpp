#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initial status bar message
    statusBar()->showMessage("Ready");

    // Connect push buttons
    connect(ui->pushButton, &QPushButton::released,
        this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
        this, &MainWindow::handleButton2);

    // Connect tree view click
    connect(ui->treeView, &QTreeView::clicked,
        this, &MainWindow::handleButton3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton()
{
    statusBar()->showMessage("Add button was clicked");
}

void MainWindow::handleButton2()
{
    statusBar()->showMessage("Second button was clicked");
}

void MainWindow::handleButton3()
{
    statusBar()->showMessage("Tree view was clicked");
}