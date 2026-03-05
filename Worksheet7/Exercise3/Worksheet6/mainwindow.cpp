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
#include <QPushButton>

// ---- VTK + Qt VTK Widget ----
#include <QVTKOpenGLNativeWidget.h>

#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

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
    ui->toolBar->addAction(ui->actionOpen_File);

    // ------------------------------------------------------------
    // Exercise 10: Context menu action on tree view
    // ------------------------------------------------------------
    ui->treeView->addAction(ui->actionItem_Options);

    // ------------------------------------------------------------
    // Status bar updates via a signal
    // ------------------------------------------------------------
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    emit statusUpdateMessage("Ready", 0);

    // ------------------------------------------------------------
    // Buttons
    // ------------------------------------------------------------
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);

    // ------------------------------------------------------------
    // Tree click
    // ------------------------------------------------------------
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    // ------------------------------------------------------------
    // Tree model population
    // ------------------------------------------------------------
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

    // ============================================================
    // Worksheet 7 Exercise 3.3.2: Rendering the cylinder in your application
    // Add this to the MainWindow constructor:
    //
    // 1) Link a render window with the Qt widget
    // 2) Add a renderer
    // 3) Create an object (copy cylinder example)
    // ============================================================

    // 1) Link a render window with the Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    // 2) Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // 3) Cylinder pipeline (copied from the earlier cylinder example)
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetResolution(8);

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cylinder->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // style similar to worksheet
    actor->GetProperty()->SetColor(1.0, 0.0, 0.35);

    renderer->AddActor(actor);
    renderer->SetBackground(0.0, 0.0, 0.0);

    // Reset camera (worksheet notes this probably becomes its own function later)
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    renderWindow->Render();
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
    } else {
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
    if (!selectedPart) {
        emit statusUpdateMessage("No item selected", 2000);
        return;
    }

    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage("Selected: " + text, 2000);
}

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

    QModelIndex current = ui->treeView->currentIndex();
    if (!current.isValid()) {
        emit statusUpdateMessage("Select a tree item first", 2000);
        return;
    }

    QString baseName = QFileInfo(fileName).fileName();
    ui->treeView->model()->setData(current.siblingAtColumn(0), baseName, Qt::EditRole);

    emit statusUpdateMessage("Renamed selected item to: " + baseName, 2000);
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex current = ui->treeView->currentIndex();
    if (!current.isValid()) {
        emit statusUpdateMessage("Right-click a tree item (select one first)", 2000);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(current.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Right-click a tree item (select one first)", 2000);
        return;
    }

    OptionDialog dialog(this);
    dialog.setFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.applyToModelPart(selectedPart);

        ui->treeView->model()->setData(current.siblingAtColumn(0), selectedPart->name(), Qt::EditRole);
        ui->treeView->model()->setData(current.siblingAtColumn(1), selectedPart->isVisible(), Qt::EditRole);

        emit statusUpdateMessage("Item Options accepted", 2000);
    } else {
        emit statusUpdateMessage("Item Options cancelled", 2000);
    }
}
