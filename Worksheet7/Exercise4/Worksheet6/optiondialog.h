#pragma once

#include <QDialog>
#include <QColor>

#include "ModelPart.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    void setFromModelPart(const ModelPart *part);
    void applyToModelPart(ModelPart *part) const;

private:
    Ui::OptionDialog *ui;
};
