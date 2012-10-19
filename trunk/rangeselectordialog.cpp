#include "rangeselectordialog.h"
#include "ui_rangeselectordialog.h"

RangeSelectorDialog::RangeSelectorDialog(QWidget *parent, QString VarName) :
    QDialog(parent),
    ui(new Ui::RangeSelectorDialog)
{
    ui->setupUi(this);
    ui->VarNameLineEdit->setText(VarName);
    ColorCombo = new ColorListEditor(this);
    ui->verticalLayout->insertWidget(0, (QWidget*)ColorCombo, Qt::AlignBottom);
}

RangeSelectorDialog::~RangeSelectorDialog()
{
    delete ui;
}

void RangeSelectorDialog::setColor(QColor Color)
{
    ColorCombo->setColor(Color);
}

QColor RangeSelectorDialog::getColor(void)
{
    return ColorCombo->color();
}

QPointF RangeSelectorDialog::getRange(void)
{
    return QPointF(ui->doubleSpinBoxMin->value(), ui->doubleSpinBoxMax->value());
}

QString RangeSelectorDialog::getVarName(void)
{
    return QString(ui->VarNameLineEdit->text());
}


double RangeSelectorDialog::getResolution(void)
{
    return ui->doubleSpinBoxRes->value();
}

void RangeSelectorDialog::setRange(QPointF RangeA)
{
    ui->doubleSpinBoxMin->setValue(RangeA.x());
    ui->doubleSpinBoxMax->setValue(RangeA.y());
}

void RangeSelectorDialog::setResolution(double Resolution)
{
    return ui->doubleSpinBoxRes->setValue(Resolution);
}
