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

void ParameterSliderDialog::getSliders(QMap <QString, QPointF> *SliderMap)
{
    for(int i = 0 ; i < SliderList.count() ; i ++)
    {
        SliderMap->insert(SliderList.at(i)->objectName(), SliderRangeMapping.value(SliderList.at(i)));
    }
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
void ParameterSliderDialog::removeSlider(QString VarName)
{
    for(int i = 0 ; i < SliderList.count() ; i ++ )
    {
        if(SliderList.at(i)->objectName() == VarName)
        {
            if(SliderRangeMapping.contains(SliderList.at(i)))
                SliderRangeMapping.remove(SliderList.at(i));
            QwtSlider *pSlider = SliderList.takeAt(i);
            delete pSlider;
            delete LableList.takeAt(i);
            delete LayoutList.takeAt(i);
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
            if(SliderRangeMapping.contains(SliderList.at(i)))
                SliderRangeMapping.remove(SliderList.at(i));
            SliderRangeMapping.insert(SliderList.at(i), RangeA);
            return;
        }
    }
    QwtSlider *pSlider = new QwtSlider(this, Qt::Vertical, QwtSlider::LeftScale, QwtSlider::BgBoth);
    pSlider->setObjectName(VarNameA);
    pSlider->setRange(RangeA.x(), RangeA.y());
    if(SliderRangeMapping.contains(pSlider))
        SliderRangeMapping.remove(pSlider);
    SliderRangeMapping.insert(pSlider, RangeA);
    QVBoxLayout *Layout = new QVBoxLayout(this);
    ui->horizontalLayout->addLayout(Layout);
    Layout->addWidget(pSlider);
    QLabel *Label = new QLabel(this);
    Label->setText(VarNameA);
    QFont Font = Label->font();
    Font.setPixelSize(20);
    Label->setFont(Font);
    Layout->addWidget(Label);

    LableList.append(Label);
    LayoutList.append(Layout);
    SliderList.append(pSlider);
    pSlider->setValue(InitialValue);

    connect(pSlider, SIGNAL(valueChanged(double)), this, SLOT(valueChanged(double )));
    connect(pSlider, SIGNAL(sliderReleased()), pParent, SLOT(markerRelease()));
    connect(pSlider, SIGNAL(sliderPressed()), pParent, SLOT(markerPress()));
    connect(pSlider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(pSlider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    emit parameterChange(VarNameA, pSlider->value());
    setMinimumWidth(SliderList.count()*100);
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
