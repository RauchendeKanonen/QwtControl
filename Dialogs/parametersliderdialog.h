#ifndef PARAMETERSLIDERDIALOG_H
#define PARAMETERSLIDERDIALOG_H


#include <qwt_slider.h>
#include <QDialog>
#include <QMap>
#include <QLabel>
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
    void setSlider(QString VarNameA, double InitialValue);
    void getSliders(QMap<QString, QPointF> *SliderMap);
    void removeSlider(QString VarName);
private:
    Ui::ParameterSliderDialog *ui;
    QList <QwtSlider*> SliderList;
    QList <QLayout*> LayoutList;
    QList <QLabel *> LableList;
    QMap <QwtSlider*, QPointF> SliderRangeMapping;
    QWidget *pParent;
public slots:
    void sliderPressed(void);
    void valueChanged ( double value );
    void sliderReleased(void);
signals:
    void parameterChange(QString VarName, double Value);
};

#endif // PARAMETERSLIDERDIALOG_H
