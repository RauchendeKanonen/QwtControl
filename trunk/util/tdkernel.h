#ifndef TDKERNEL_H
#define TDKERNEL_H

#include <QCoreApplication>
#include "mathFunction/mathfunctioncompiler.h"
#include "mathFunction/mathfunctionevaluator.h"
#include "numericallaplace.h"
#include "mathFunction/controlexpression.h"

#include <QtGui/QApplication>
#include "muParser.h"
#include "mpParser.h"

class TDKernel
{
public:
    bool genKernel(mathFunctionEvaluator *Eval, double dt, int Length);
    double convolvedDot(size_t n, QList<double> *Signal);
    double convolvedDot(size_t n, mup::Value *X);
    TDKernel(mathFunctionEvaluator *pEval, double dtA, size_t len);
    QPolygonF getKernel(void);


private:
    mathFunctionEvaluator *Evaluator;
    QPolygonF Kernel;
    double dt;
    size_t KernelLength;
};


#endif // TDKERNEL_H
