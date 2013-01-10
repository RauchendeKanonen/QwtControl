#include "parametersliderdialog.h"
#include "ui_parametersliderdialog.h"
#include <QLabel>
#include <qwt_slider.h>
#include <QRect>
#include <QFont>

ParameterSliderDialog::ParameterSliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterSliderDialog)
{
    pParent = parent;
    ui->setupUi(this);
}

ParameterSliderDialog::~ParameterSliderDialog()
{
    delete ui;
}

void ParameterSliderDialog::setSlider(QString VarNameA, double InitialValue)
{
    for(int i = 0 ; i < SliderList.count() ; i ++ )
    {
        if(SliderList.at(i)->objectName() == VarNameA)
        {
            SliderList.at(i)->setValue(InitialValue);
            return;
        }
    }
}

void ParameterSliderDialog::addSlider(QString VarNameA, QPointF RangeA, double InitialValue)
{
    for(int i = 0 ; i < SliderList.count() ; i ++ )
    {
        if(SliderList.at(i)->objectName() == VarNameA)
        {
            SliderList.at(i)->setRange(RangeA.x(), RangeA.y());
            return;
        }
    }
    QwtSlider *pSlider = new QwtSlider(this, Qt::Vertical, QwtSlider::LeftScale, QwtSlider::BgBoth);
    pSlider->setObjectName(VarNameA);
    pSlider->setRange(RangeA.x(), RangeA.y());
    QVBoxLayout *Layout = new QVBoxLayout(this);
    ui->horizontalLayout->addLayout(Layout);
    Layout->addWidget(pSlider);
    QLabel *Label = new QLabel(this);
    Label->setText(VarNameA);
    QFont Font = Label->font();
    Font.setPixelSize(20);
    Label->setFont(Font);
    Layout->addWidget(Label);


    SliderList.append(pSlider);
    pSlider->setValue(InitialValue);

    connect(pSlider, SIGNAL(valueChanged(double)), this, SLOT(valueChanged(double )));
    connect(pSlider, SIGNAL(sliderReleased()), pParent, SLOT(markerRelease()));
    connect(pSlider, SIGNAL(sliderPressed()), pParent, SLOT(markerPress()));
    connect(pSlider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(pSlider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    emit parameterChange(VarNameA, pSlider->value());
    setMinimumWidth(SliderList.count()*70);
    adjustSize();
}

void ParameterSliderDialog::sliderPressed(void)
{

}

void ParameterSliderDialog::sliderReleased(void)
{
    QwtSlider *Slider = (QwtSlider*)QObject::sender();


    QString VarName = Slider->objectName();
    double value = Slider->value();
    double Parameter = value;
    emit parameterChange(VarName, Parameter);

}

void ParameterSliderDialog::valueChanged ( double value )
{
    QwtSlider *Slider = (QwtSlider*)QObject::sender();

    QString VarName = Slider->objectName();
    double Parameter = value;
    emit parameterChange(VarName, Parameter);
}
