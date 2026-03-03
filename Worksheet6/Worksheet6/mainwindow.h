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
    void handleTreeClicked(const QModelIndex& index);

    void on_actionOpen_File_triggered();

    // Exercise 10: context menu action slot
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow* ui;
    ModelPartList* partList;
};
