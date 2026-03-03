#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    // Sensible defaults (in case no part is selected)
    ui->spinBoxR->setRange(0, 255);
    ui->spinBoxG->setRange(0, 255);
    ui->spinBoxB->setRange(0, 255);
    ui->checkBoxVisible->setChecked(true);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setFromModelPart(const ModelPart *part)
{
    if (!part)
        return;

    ui->lineEditName->setText(part->name());

    QColor c = part->colour();
    ui->spinBoxR->setValue(c.red());
    ui->spinBoxG->setValue(c.green());
    ui->spinBoxB->setValue(c.blue());

    ui->checkBoxVisible->setChecked(part->isVisible());
}

void OptionDialog::applyToModelPart(ModelPart *part) const
{
    if (!part)
        return;

    part->setName(ui->lineEditName->text());

    QColor c(ui->spinBoxR->value(), ui->spinBoxG->value(), ui->spinBoxB->value());
    part->setColour(c);

    part->setIsVisible(ui->checkBoxVisible->isChecked());
}
