#include "rangeselectordialog.h"
#include "ui_rangeselectordialog.h"

RangeSelectorDialog::RangeSelectorDialog(QWidget *parent, QString VarName) :
    QDialog(parent),
    ui(new Ui::RangeSelectorDialog)
{
    ui->setupUi(this);
    VarComboBox = NULL;
    ui->VarNameLineEdit->setText(VarName);
    ColorCombo = new ColorListEditor(this);
    ui->verticalLayout->insertWidget(0, (QWidget*)ColorCombo, Qt::AlignBottom);
}


RangeSelectorDialog::RangeSelectorDialog(QWidget *parent, QStringList VarNames) :
    QDialog(parent),
    ui(new Ui::RangeSelectorDialog)
{
    ui->setupUi(this);
    VarComboBox = new QComboBox();
    for(int i = 0 ; i < VarNames.count(); i++)
        VarComboBox->insertItem(0, VarNames.at(i));
    ui->verticalLayout->insertWidget(1, VarComboBox, Qt::AlignBottom);

    connect(VarComboBox, SIGNAL(currentIndexChanged(const QString)), this, SLOT(variableSelected(const QString)));

    ColorCombo = new ColorListEditor(this);
    ui->verticalLayout->insertWidget(0, (QWidget*)ColorCombo, Qt::AlignBottom);
}

void RangeSelectorDialog::variableSelected(const QString Index)
{
    ui->VarNameLineEdit->setText(Index);
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
