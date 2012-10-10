#include "qparameterslider.h"

QParameterSlider::QParameterSlider(QWidget *parent, QString VarNameA) :
    QSlider(parent)
{
    VarName = VarNameA;
    ValueLabel = new QLabel(this);
}
