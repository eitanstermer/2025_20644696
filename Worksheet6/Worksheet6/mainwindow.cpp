#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::released,
        this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
        this, &MainWindow::handleButton2);

    connect(ui->treeView, &QTreeView::clicked,
        this, &MainWindow::handleButton3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton()
{
    QMessageBox msgBox;
    msgBox.setText("Add button was clicked");
    msgBox.exec();
}

void MainWindow::handleButton2()
{
    QMessageBox msgBox;
    msgBox.setText("Second button was clicked");
    msgBox.exec();
}

void MainWindow::handleButton3()
{
    QMessageBox msgBox;
    msgBox.setText("Tree view was clicked");
    msgBox.exec();
}