#ifndef NUMERICALLAPLACE_H
#define NUMERICALLAPLACE_H
#include <math.h>
#include "mpParser.h"
#include "mathFunction/mathfunctionevaluator.h"
using namespace mup;

typedef double FunktionPrt(double t);
#define DefaultStehfestN 14

class NumericalLaplace
{
private:
    double *V;       //  Stehfest coefficients
    double ln2;       //  log of 2
    int V_Length;

public:
    NumericalLaplace();
    ~NumericalLaplace();
    double InverseTransform(ParserX *f, double t);
    double InverseTransform(mathFunctionEvaluator *Eval, double t);
    double Factorial(int N);
    void InitStehfest(int N);
};

#endif // NUMERICALLAPLACE_H
