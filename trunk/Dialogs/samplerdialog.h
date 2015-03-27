#ifndef SAMPLERDIALOG_H
#define SAMPLERDIALOG_H

#include <QDialog>
#include "mathfunctionevaluator.h"
#include "definitions.h"
#include "qwt_control_plot_item.h"
#include "qwt_response_curve.h"
#include <iostream>
#include <string>
#include <ginac/ginac.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "controlexpression.h"
using namespace std;
using namespace GiNaC;
namespace Ui {
class SamplerDialog;
}

class SamplerDialog : public QDialog
{
    Q_OBJECT

public:
    ex inversZTransform(QString RationalFunction);
    explicit SamplerDialog(QWidget *parent = 0);
    void insertCurve(QwtControlPlotItem *Item);
    ~SamplerDialog();
    void reSample(void);
public slots:

    void dataSlot(QPolygonF PolygonA);
private slots:
    void on_pushButtonSample_clicked();

    void on_pushButtonXmGrace_clicked();

    void on_pushButtonDeq_clicked();
    void on_pushButtonCSV_clicked();

signals:
    void newExpression(QString);
private:
    double *XData;
    double *YData;
    QPolygonF SrcPolygon;
    QPolygonF SampledPolygon;
    Ui::SamplerDialog *ui;
    QwtPlotCurve *Curve;
    mathFunctionEvaluator *Eval;
};

#endif // SAMPLERDIALOG_H
