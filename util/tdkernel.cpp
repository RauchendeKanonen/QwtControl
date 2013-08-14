#include "tdkernel.h"


bool TDKernel::genKernel(mathFunctionEvaluator *Eval, double dt, int Length)
{
    Kernel.clear();
    NumericalLaplace NLaplace(Eval, TRANSFORM_WEEKS);
    double t = 0.0;
    double Integral = 0;

    Kernel = NLaplace.InverseTransform(dt, Length*dt, 0);


    for(int i = 0 ; i < Length ; i++ )
    {
        t += dt;
        Integral += Kernel.at(i).y();
    }

    for(int i = 0 ; i < Length ; i++ )
    {
        Kernel.replace(i,Kernel.at(i)/Integral);
    }

    return true;
}

double TDKernel::convolvedDot(size_t n, QList<double> *Signal)
{
    double Result;
    size_t SignalLen = Signal->count();
    size_t KernelLen = Kernel.count();
    size_t kmin, kmax, k;

    Result = 0;

    kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
    kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

    for (k = kmin; k <= kmax; k++)
    {
        Result += Signal->at(k) * Kernel.at(n - k).y();
    }

    return Result;
}

double TDKernel::convolvedDot(size_t n, mup::Value *X)
{
    double Result;
    size_t SignalLen = X->GetDim();
    size_t KernelLen = Kernel.count();
    size_t kmin, kmax, k;



    Result = 0;

    kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
    kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

    for (k = kmin; k <= kmax; k++)
    {
        double Input = (X->At(k).GetFloat()) ;
        Result += (X->At(k).GetFloat()) * Kernel.at(n - k).y();
    }

    return Result;
}

TDKernel::TDKernel(mathFunctionEvaluator *pEval, double dtA, size_t len)
{
    Evaluator = pEval;
    dt = dtA;
    KernelLength = len;
    genKernel(Evaluator, dt, KernelLength);
}

QPolygonF TDKernel::getKernel(void)
{
    return Kernel;
}
