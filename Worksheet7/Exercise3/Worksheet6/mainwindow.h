#pragma once

#include <QMainWindow>
#include <QString>
#include <QModelIndex>

// Your existing includes
#include "ModelPart.h"
#include "ModelPartList.h"

// VTK smart pointer + forward decls
#include <vtkSmartPointer.h>

class vtkRenderer
    ;
class vtkGenericOpenGLRenderWindow
    ;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr)
        ;
    ~MainWindow()
        ;

signals:
    void statusUpdateMessage(const QString& message, int timeout)
        ;

public slots:
    void handleButton()
        ;
    void handleButton2()
        ;
    void handleTreeClicked(const QModelIndex& index)
        ;

    void on_actionOpen_File_triggered()
        ;

    void on_actionItem_Options_triggered()
        ;

private:
    Ui::MainWindow* ui
        ;
    ModelPartList* partList
        ;

    // VTK members
    vtkSmartPointer<vtkRenderer> renderer
        ;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow
        ;
}
;