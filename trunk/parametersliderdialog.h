#ifndef PARAMETERSLIDERDIALOG_H
#define PARAMETERSLIDERDIALOG_H
#include "qparameterslider.h"


#include <QDialog>

namespace Ui {
class ParameterSliderDialog;
}

class ParameterSliderDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParameterSliderDialog(QWidget *parent = 0);
    ~ParameterSliderDialog();
    void addSlider(QString VarNameA, QPointF RangeA, double InitialValue=0.0);
private:
    Ui::ParameterSliderDialog *ui;
    QList <QParameterSlider*> SliderList;
public slots:
    void sliderPressed(void);
    void valueChanged ( int value );
    void sliderReleased(void);
signals:
    void parameterChange(QString VarName, double Value);
};

#endif // PARAMETERSLIDERDIALOG_H
