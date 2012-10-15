#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <QSemaphore>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include "mpParser.h"
#include "mathFunction/mathfunctionevaluator.h"
#include "varmodel.h"
#include "qwt_plot_control_curve.h"

using namespace mup;
#define CURVE_TYPE_XY           1
#define CURVE_TYPE_COMPLEX      2
#define CURVE_TYPE_MAGNITUDE    3
#define CURVE_TYPE_XY_COMPILED_NUMERICAL 5


#define LINUX 1


struct CurveInformationStruct
{
    QSemaphore *Sem;
    int CurveType;
    char IndepVarName[32];
    double MarkerPos;
    double StartPoint;
    double EndPoint;
    double Resolution;
    ParserX *Expression;
    ParserX *BaseExpression;
    mathFunctionEvaluator *Evaluator;
    QwtPlotControlCurve *Curve;
    double *xData;
    double *yData;
    int DataSize;
    QwtPlot *Plot;
    QThread *Thread;
    VarModel *pVariabelMdl;
    QColor Color;
    QwtPlotMarker *Marker;
    QwtPlotMarker *PoleLocation;
    QwtPlotMarker *RootLocation;
};


#endif // DEFINITIONS_H
