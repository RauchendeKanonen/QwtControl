#include <QCoreApplication>
#include "mathFunction/mathfunctioncompiler.h"
#include "mathFunction/mathfunctionevaluator.h"
#include "numericallaplace.h"


double *convolve(const double *Signal, size_t SignalLen,
                 const double *Kernel, size_t KernelLen)
{
    double *Result = (double*)malloc(sizeof(double)*(SignalLen + KernelLen));
    size_t n;

    double KerSum = 0;

    for ( int i = 0 ; i < KernelLen ; i ++ )
    {
        KerSum += Kernel[i];
    }
    for (n = 0; n < SignalLen + KernelLen - 1; n++)
    {
        size_t kmin, kmax, k;

        Result[n] = 0;

        kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
        kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

        for (k = kmin; k <= kmax; k++)
        {
            Result[n] += Signal[k] * Kernel[n - k] / KerSum;
        }

    }
    return Result;
}

double convolvedDot(size_t n, const double *Signal, size_t SignalLen,
                 const double *Kernel, size_t KernelLen)
{
    double Result;

    size_t kmin, kmax, k;

    Result = 0;

    kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
    kmax = (n < SignalLen - 1) ? n : SignalLen - 1;


    for (k = kmin; k <= kmax; k++)
    {
        Result += Signal[k] * Kernel[n - k];
    }

    return Result;
}

int genKernel(QString FunktionDef, double dt, int Length, double *Kernel)
{
    mathFunctionCompiler *Compiler = new mathFunctionCompiler(FunktionDef, "s", "sorder");
    mathFunctionEvaluator *Eval = new mathFunctionEvaluator("s", "sorder");
    Eval->setVar("a", 100);
    Eval->setVar("b", 10);
    NumericalLaplace NLaplace;
    double t = 0.0;


    for(int i = 0 ; i < Length ; i++ )
    {
        t += dt;
        *(Kernel+i) = NLaplace.InverseTransform(Eval, t);
    }
    delete Eval;
    delete Compiler;
    return 0;
}

void putKernel(double *Kernel, int Length)
{
    for(int i = 0 ; i < Length ; i++ )
        printf("%i %f\n", i, *(Kernel+i));
}

int main(int argc, char *argv[])
{

    int Length = 10000;
    double dt = 0.1;
    double Kernel[Length];
    genKernel("1/(pow(s,2.0)+1/a*s+1)", dt, Length, Kernel);


    int SignalLength = 100000;
    double Signal[SignalLength];
    for(int i = 0 ; i < SignalLength ; i ++)
    {
        Signal[i] = 1;
    }

    //double *Convolved = convolve(Signal, SignalLength, Kernel, Length);

    double SingleConvolved[Length];


    for(int i = 0 ; i < SignalLength ; i ++)
    {
        SingleConvolved[i] = convolvedDot(i, Signal, SignalLength, Kernel, Length);
    }


    //putKernel(Convolved, SignalLength);
    //printf("\n");
    putKernel(SingleConvolved, SignalLength);
}
