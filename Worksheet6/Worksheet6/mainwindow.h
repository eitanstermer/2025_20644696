#pragma once

#include <QMainWindow>
#include <QString>
#include <QModelIndex>

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

    // Must match QTreeView::clicked(const QModelIndex&)
    void handleTreeClicked(const QModelIndex& index);

    // QAction objectName: actionOpen_File
    void on_actionOpen_File_triggered();

private:
    Ui::MainWindow* ui;
    ModelPartList* partList;
};
