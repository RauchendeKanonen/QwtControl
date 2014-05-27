#include "transformtypedialog.h"
#include "ui_transformtypedialog.h"
#include "numericallaplace.h"
TransformTypeDialog::TransformTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransformTypeDialog)
{
    ui->setupUi(this);
}

TransformTypeDialog::~TransformTypeDialog()
{
    delete ui;
}


int TransformTypeDialog::getAlgorithm(void)
{
    if(ui->checkBoxDubner->isChecked())
        return TRANSFORM_DUBBNERABATE;
    if(ui->checkBoxGaverStehfest->isChecked())
        return TRANSFORM_GAVER_STEHFEST;
    if(ui->checkBoxWeeks->isChecked())
        return TRANSFORM_WEEKS;

    return TRANSFORM_WEEKS;
}
