#ifndef STEPRESPONSEDIALOG_H
#define STEPRESPONSEDIALOG_H

#include <QDialog>
#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qmath.h>
#include "mpParser.h"
#include "qwt_plot_control_curve.h"
using namespace mup;
namespace Ui
{
class StepResponseDialog;
}

class StepResponseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StepResponseDialog(QWidget *parent = 0);
    ~StepResponseDialog();
    void setResponse(const double *XData, const double *YData, int Size);
    void setExpression(ParserX *ExpressionA);
    void replot(void);
    QwtPlotControlCurve *getPlotCurve(void);
    QwtPlot *getPlot(void);
private:
    double Time[10000];
    double ResponseValue[10000];
    QWidget *pParent;
    Ui::StepResponseDialog *ui;
    QwtPlotControlCurve *ResponseCurve;
    ParserX *Expression;
};

#endif // STEPRESPONSEDIALOG_H
