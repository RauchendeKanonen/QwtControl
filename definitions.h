#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <QSemaphore>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>

#include "mathFunction/mathfunctionevaluator.h"
#include "varmodel.h"





#define LINUX 1

typedef struct
{
    double IndepStart;
    double IndepEnd;
    double Resolution;
    int Dots;
}EvalInfo;

#define Rtti_PlotRootLocus 1
#define Rtti_PlotResponse  2
#define Rtti_PlotDataSet   3
#define Rtti_PlotPhase     4
#define Rtti_PlotMagnitude 5
#define Rtti_PlotNumericRootLocus 6
#define Rtti_PlotDiscreteResponse 7
#endif // DEFINITIONS_H
