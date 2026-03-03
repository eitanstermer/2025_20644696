#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QTreeView>
#include <QModelIndex>
#include <QString>
#include <QIcon>
#include <QSize>
#include <QFileDialog>
#include <QFileInfo>

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

    // Force the action onto the toolbar (Designer can be flaky)
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
    // clicked(const QModelIndex&) -> slot must accept QModelIndex
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

void MainWindow::handleTreeClicked(const QModelIndex& index)
{
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 2000);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("No item selected", 2000);
        return;
    }

    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage("Selected: " + text, 2000);
}

// IMPORTANT:
// This slot name MUST match the objectName of the QAction in the .ui file.
// Your action is named: actionOpen_File
// Therefore the auto-connected slot should be:
void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt)")
        );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("Open cancelled", 2000);
        return;
    }

    emit statusUpdateMessage("Selected: " + fileName, 2000);

    // Sign-off behaviour: rename currently selected tree item to chosen filename
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No tree item selected to rename", 2000);
        return;
    }

    QString baseName = QFileInfo(fileName).fileName();

    // Preferred: go through the model (if setData is implemented there)
    bool changed = false;
    if (ui->treeView->model()) {
        changed = ui->treeView->model()->setData(index, baseName, Qt::EditRole);
    }

    // Fallback: update the underlying item directly (your ModelPart supports this)
    if (!changed) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        if (!selectedPart) {
            emit statusUpdateMessage("Could not access selected item", 2000);
            return;
        }

        // ModelPart::setData(int column, const QVariant& value)
        selectedPart->setData(0, baseName);

        // Force view repaint (model is not emitting dataChanged yet)
        ui->treeView->viewport()->update();

        emit statusUpdateMessage("Renamed selected item to: " + baseName, 2000);
    }
}
