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

    // Button connects (still required by signoff: at least one button does something)
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);

    // Tree click connect for Exercise 5
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    // ============================================================
    // Exercise 4: Working Tree View (worksheet demo code)
    // ============================================================

    /* Create/allocate the ModelList */
    this->partList = new ModelPartList("Parts List");

    /* Link it to the tree view in the GUI */
    ui->treeView->setModel(this->partList);

    /* Manually create a model tree (worksheet quick example) */
    ModelPart *rootItem = this->partList->getRootItem();

    /* Add 3 top level items */
    for (int i = 0; i < 3; i++) {

        /* Create strings for both data columns */
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        /* Create child item */
        ModelPart *childItem = new ModelPart({name, visible}, rootItem);

        /* Append to tree top-level */
        rootItem->appendChild(childItem);

        /* Add 5 sub-items */
        for (int j = 0; j < 5; j++) {

            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");

            ModelPart *childChildItem = new ModelPart({subName, subVisible}, childItem);

            /* Append to parent */
            childItem->appendChild(childChildItem);
        }
    }

    // Expand tree so you can see it immediately
    ui->treeView->expandAll();
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
    // ============================================================
    // Exercise 5: Determine user's Tree View Selection (worksheet code)
    // ============================================================

    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* Retrieve the name string from the internal QVariant data array */
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}