#ifndef NUMERICALLAPLACE_H
#define NUMERICALLAPLACE_H
#include <math.h>
#include <QPolygonF>
#include "mathFunction/mathfunctionevaluator.h"


typedef double FunktionPrt(double t);
#define TRANSFORM_GAVER_STEHFEST 1
#define TRANSFORM_WEEKS          2
#define NR_END 1
#define FREE_ARG char*

#define DefaultStehfestN 25

class NumericalLaplace
{
private:
    double *V;       //  Stehfest coefficients
    double ln2;       //  log of 2
    int V_Length;
    int TransformType;
    mathFunctionEvaluator *Eval;
public:
    NumericalLaplace(mathFunctionEvaluator *EvalA, int TransformType=TRANSFORM_GAVER_STEHFEST);
    ~NumericalLaplace();
    double acosh(double x);
    double U(double x);
    double *davec(long nl, long nh);
    void dfvec(double *v, long nl, long nh);
    double cotan(double i);
    static void aerror(char error_text[]);
    QPolygonF linvweex(mathFunctionEvaluator *EvalA,
                 int p,
                 double c,
                 double grosst,
                 double rho,
                 int m,
                 double *t,
                 double *a);

    double auinvlap(int n,
                    double *a,
                    double c,
                    double t,
                    double grosst);
    void umkehr(int *m,
                int n,
                int i,
                int *j,
                int *k,
                int nh,
                int nq,
                int ne);
    void xfft(int p,
              int analyse,
              double *u,
              double *v);

    double Round(double x);
    QPolygonF InverseTransform(double dt, double tEnd);
    double Factorial(int N);
    void InitStehfest(int N);
};

#endif // NUMERICALLAPLACE_H
