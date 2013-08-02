#include "controlsystemtracker.h"
#include "ui_controlsystemtracker.h"

ControlSystemTracker::ControlSystemTracker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlSystemTracker)
{
    ui->setupUi(this);
}

ControlSystemTracker::~ControlSystemTracker()
{
    delete ui;
}


void ControlSystemTracker::SystemProperty(double Value, QString PropertyName)
{
    if(PropertyName == "Overshoot")
        ui->lcdNumberOvershoot->display(Value);
    if(PropertyName == "Omega")
        ui->lcdNumberW->display(Value);
    if(PropertyName == "Damping")
        ui->lcdNumberDamping->display(Value);
    if(PropertyName == "SettlingTime")
        ui->lcdNumberSettlingTime->display(Value);

}
