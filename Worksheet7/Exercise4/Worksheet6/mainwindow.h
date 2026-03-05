#pragma once

#include <QMainWindow>
#include <QString>
#include <QModelIndex>

#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

#include "ModelPart.h"
#include "ModelPartList.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void statusUpdateMessage(const QString& message, int timeout);

public slots:
    void handleButton();
    void handleButton2();
    void handleTreeClicked(const QModelIndex& index);

    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();

private:
    // ===== Worksheet 7 Exercise 4 additions =====
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);

private:
    Ui::MainWindow* ui;
    ModelPartList* partList;

    // ===== Worksheet 7 Exercise 3/4: VTK renderer objects =====
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
};
