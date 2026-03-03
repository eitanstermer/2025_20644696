#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QTreeView>
#include <QModelIndex>
#include <QString>
#include <QIcon>
#include <QSize>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , partList(nullptr)
{
    ui->setupUi(this);

    // ------------------------------------------------------------
    // Exercise 6/7: Toolbar + icon from Qt resources
    // ------------------------------------------------------------
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->toolBar->setIconSize(QSize(32, 32));
    ui->actionOpen_File->setIcon(QIcon(":/icons/open.png"));

    // Make sure the action is actually on the toolbar
    // (Designer can be flaky; this forces it)
    ui->toolBar->addAction(ui->actionOpen_File);

    // ------------------------------------------------------------
    // Worksheet pattern: status bar updates via a signal
    // ------------------------------------------------------------
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);

    emit statusUpdateMessage("Ready", 0);

    // ------------------------------------------------------------
    // Buttons (Ex1/2 signoff behaviour)
    // ------------------------------------------------------------
    connect(ui->pushButton, &QPushButton::released,
        this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
        this, &MainWindow::handleButton2);

    // ------------------------------------------------------------
    // Tree click (Ex5)
    // ------------------------------------------------------------
    connect(ui->treeView, &QTreeView::clicked,
        this, &MainWindow::handleTreeClicked);

    // ------------------------------------------------------------
    // Exercise 4: Tree model population
    // ------------------------------------------------------------
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    ModelPart* rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible }, rootItem);
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");

            ModelPart* childChildItem = new ModelPart({ subName, subVisible }, childItem);
            childItem->appendChild(childChildItem);
        }
    }

    ui->treeView->expandAll();
}

MainWindow::~MainWindow()
{
    delete partList;
    delete ui;
}

void MainWindow::handleButton()
{
    emit statusUpdateMessage("Add button was clicked", 2000);
}

void MainWindow::handleButton2()
{
    emit statusUpdateMessage("Second button was clicked", 2000);
}

void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart) {
        emit statusUpdateMessage("No item selected", 2000);
        return;
    }

    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage("Selected: " + text, 2000);
}

void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage("Open File action triggered", 2000);
}