#include "parametersliderdialog.h"
#include "ui_parametersliderdialog.h"
#include <QLabel>


ParameterSliderDialog::ParameterSliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterSliderDialog)
{
    ui->setupUi(this);
}

ParameterSliderDialog::~ParameterSliderDialog()
{
    delete ui;
}

void ParameterSliderDialog::addSlider(QString VarNameA, QPointF RangeA, double InitialValue)
{
    for(int i = 0 ; i < SliderList.count() ; i ++ )
    {
        if(SliderList.at(i)->GetName() == VarNameA)
        {
            SliderList.at(i)->setRange(RangeA.x()*10e6, RangeA.y()*10e6);
            return;
        }
    }

    QParameterSlider *Slider = new QParameterSlider(this, VarNameA);
    QVBoxLayout *Layout = new QVBoxLayout(this);
    ui->horizontalLayout->addLayout(Layout);
    Layout->addWidget(Slider);
    QLabel *Label = new QLabel(this);
    Label->setText(VarNameA);
    Layout->addWidget(Label);


    SliderList.append(Slider);
    Slider->setMaximum(RangeA.y()*10e6);
    Slider->setMinimum(RangeA.x()*10e6);
    Slider->setValue(InitialValue*10e6);
    connect(Slider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int )));
    connect(Slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));

    QLabel *ValueLabel = Slider->getLabel();
    ValueLabel->setText(QString().sprintf("%f", Slider->value()/10e6));
    Layout->addWidget(ValueLabel);
    emit parameterChange(VarNameA, Slider->value()/10e6);
    adjustSize();
}


void ParameterSliderDialog::sliderReleased(void)
{
    QParameterSlider *Slider = (QParameterSlider*)QObject::sender();

    QLabel *ValueLabel = Slider->getLabel();
    ValueLabel->setText(QString().sprintf("%f", Slider->value()/10e6));

    QString VarName = Slider->GetName();
    int value = Slider->value();
    double Parameter = value/10e6;
    emit parameterChange(VarName, Parameter);

}

void ParameterSliderDialog::valueChanged ( int value )
{
    QParameterSlider *Slider = (QParameterSlider*)QObject::sender();

    QLabel *ValueLabel = Slider->getLabel();
    ValueLabel->setText(QString().sprintf("%f", Slider->value()/10e6));

    QString VarName = Slider->GetName();
    double Parameter = value/10e6;
    emit parameterChange(VarName, Parameter);
}
