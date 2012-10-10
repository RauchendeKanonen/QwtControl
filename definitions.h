#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <QSemaphore>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include "mpParser.h"
using namespace mup;
#define CURVE_TYPE_XY           1
#define CURVE_TYPE_COMPLEX      2
#define CURVE_TYPE_MAGNITUDE    3
#define CURVE_TYPE_XY_NUMERICAL 4
#define LINUX 1

#define N_SUBSEGMENTS -1
#define NO_SUBSEGMENTS -2
#define NumThreads 3

struct CurveInformationStruct
{
    QSemaphore *Sem;
    int CurveType;
    char IndepVarName[32];
    double MarkerPos;
    QwtPlotMarker *Marker;
    double StartPoint;
    double EndPoint;
    double Resolution;
    ParserX *Expression;
    ParserX *BaseExpression;
    QwtPlotCurve *Curve;
    double *xData;
    double *yData;
    int DataSize;
    QwtPlot *Plot;
    QThread *Thread;
    int Segment;
    CurveInformationStruct **SubSegmentInformation;
    CurveInformationStruct *ParentInformation;
};


#endif // DEFINITIONS_H
