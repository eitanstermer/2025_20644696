#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "optiondialog.h"

#include <QStatusBar>
#include <QTreeView>
#include <QModelIndex>
#include <QString>
#include <QIcon>
#include <QSize>
#include <QFileDialog>
#include <QFileInfo>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , partList(nullptr)
{
    ui->setupUi(this);

    ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->toolBar->setIconSize(QSize(32, 32));
    ui->actionOpen_File->setIcon(QIcon(":/icons/open.png"));
    ui->toolBar->addAction(ui->actionOpen_File);

    ui->treeView->addAction(ui->actionItem_Options);

    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);

    emit statusUpdateMessage("Ready", 0);

    connect(ui->pushButton, &QPushButton::released,
        this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
        this, &MainWindow::handleButton2);

    connect(ui->treeView, &QTreeView::clicked,
        this, &MainWindow::handleTreeClicked);

    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    ModelPart* rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        bool visible = true;

        ModelPart* childItem = new ModelPart({ name, visible }, rootItem);
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            bool subVisible = true;

            ModelPart* childChildItem = new ModelPart({ subName, subVisible }, childItem);
            childItem->appendChild(childChildItem);
        }
    }

    ui->treeView->expandAll();

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    renderer->SetBackground(0.0, 0.0, 0.0);
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
    QModelIndex current = ui->treeView->currentIndex();
    if (!current.isValid()) {
        emit statusUpdateMessage("Select a tree item first", 2000);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(current.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Select a tree item first", 2000);
        return;
    }

    OptionDialog dialog(this);
    dialog.setFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.applyToModelPart(selectedPart);

        ui->treeView->model()->setData(current.siblingAtColumn(0), selectedPart->name(), Qt::EditRole);
        ui->treeView->model()->setData(current.siblingAtColumn(1), selectedPart->isVisible(), Qt::EditRole);

        emit statusUpdateMessage("Dialog accepted", 2000);
    }
    else {
        emit statusUpdateMessage("Dialog rejected", 2000);
    }
}

void MainWindow::handleTreeClicked(const QModelIndex& index)
{
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 2000);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage("Selected: " + text, 2000);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open STL File"),
        "C:\\",
        tr("STL Files (*.stl)")
    );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("Open cancelled", 2000);
        return;
    }

    QModelIndex current = ui->treeView->currentIndex();

    ModelPart* parentPart = nullptr;

    if (current.isValid()) {
        parentPart = static_cast<ModelPart*>(current.internalPointer());
    }

    if (!parentPart) {
        parentPart = partList->getRootItem();
    }

    QString baseName = QFileInfo(fileName).fileName();

    ModelPart* newPart = new ModelPart({ baseName, true }, parentPart);
    parentPart->appendChild(newPart);

    newPart->loadSTL(fileName);

    ui->treeView->expandAll();

    updateRender();

    emit statusUpdateMessage("Loaded: " + baseName, 2000);
}

void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();

    updateRenderFromTree(partList->index(0, 0, QModelIndex()));

    renderer->ResetCamera();

    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid()) {

        ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

        if (part && part->isVisible()) {

            auto actor = part->getActor();

            if (actor) {
                renderer->AddActor(actor);
            }
        }
    }

    if (!partList->hasChildren(index))
        return;

    int rows = partList->rowCount(index);

    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex current = ui->treeView->currentIndex();

    if (!current.isValid()) {
        emit statusUpdateMessage("Right-click a tree item (select one first)", 2000);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(current.internalPointer());

    OptionDialog dialog(this);
    dialog.setFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {

        dialog.applyToModelPart(selectedPart);

        ui->treeView->model()->setData(current.siblingAtColumn(0), selectedPart->name(), Qt::EditRole);
        ui->treeView->model()->setData(current.siblingAtColumn(1), selectedPart->isVisible(), Qt::EditRole);

        updateRender();

        emit statusUpdateMessage("Item Options accepted", 2000);
    }
    else {
        emit statusUpdateMessage("Item Options cancelled", 2000);
    }
}