#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTreeView>
#include <QStatusBar>
#include <QModelIndex>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , partList(nullptr)
{
    ui->setupUi(this);

    // Link our signal to the status bar slot (worksheet pattern)
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    emit statusUpdateMessage("Ready", 0);

    // Buttons (signoff requires at least one button does something)
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);

    // Tree click for Exercise 5
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    // ============================================================
    // Exercise 4: Working Tree View (worksheet demo code)
    // ============================================================

    this->partList = new ModelPartList("Parts List");
    ui->treeView->setModel(this->partList);

    ModelPart *rootItem = this->partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart *childItem = new ModelPart({name, visible}, rootItem);
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");

            ModelPart *childChildItem = new ModelPart({subName, subVisible}, childItem);
            childItem->appendChild(childChildItem);
        }
    }

    ui->treeView->expandAll();

    // NOTE: Worksheet says if you follow naming convention
    // actionOpen_File + on_actionOpen_File_triggered(), Qt auto-connects.
    // So we do NOT add a manual connect() here. :contentReference[oaicite:13]{index=13}
}

MainWindow::~MainWindow()
{
    delete partList;
    delete ui;
}

void MainWindow::handleButton()
{
    emit statusUpdateMessage("Add button was clicked", 0);
}

void MainWindow::handleButton2()
{
    emit statusUpdateMessage("Second button was clicked", 0);
}

void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

// ============================================================
// Exercise 6: Open File action slot
// Worksheet requires a statusbar message when action is triggered :contentReference[oaicite:14]{index=14}
// ============================================================
void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage(QString("Open File action triggered"), 0);
}