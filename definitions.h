#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <QSemaphore>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>

#include "mathFunction/mathfunctionevaluator.h"
#include "varmodel.h"




#define CURVE_TYPE_XY           1
#define CURVE_TYPE_COMPLEX      2
#define CURVE_TYPE_MAGNITUDE    3
#define CURVE_TYPE_XY_SET       4
#define CURVE_TYPE_XY_COMPILED_NUMERICAL 5


#define LINUX 1

typedef struct
{
    double IndepStart;
    double IndepEnd;
    double Resolution;
}EvalInfo;

#define Rtti_PlotRootLocus 1
#define Rtti_PlotResponse  2
#define Rtti_PlotDataSet   3

#endif // DEFINITIONS_H
