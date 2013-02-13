#include <QCoreApplication>
#include "mathFunction/mathfunctioncompiler.h"
#include "mathFunction/mathfunctionevaluator.h"
#include "numericallaplace.h"



class TDKernel
{
public:
    bool genKernel(mathFunctionEvaluator *Eval, double dt, int Length)
    {
        Kernel->clear();
        NumericalLaplace NLaplace;
        double t = 0.0;
        double Integral = 0;

        for(int i = 0 ; i < Length ; i++ )
        {
            t += dt;
            double Value = NLaplace.InverseTransform(Eval, t);
            Kernel->append(Value);
            Integral += Value;
        }

        for(int i = 0 ; i < Length ; i++ )
        {
            Kernel->replace(i,Kernel->at(i)/Integral);
        }

        return true;
    }

    double convolvedDot(size_t n, QList<double> *Signal)
    {
        double Result;
        size_t SignalLen = Signal->count();
        size_t KernelLen = Kernel->count();
        size_t kmin, kmax, k;

        Result = 0;

        kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
        kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

        for (k = kmin; k <= kmax; k++)
        {
            Result += Signal->at(k) * Kernel->at(n - k);
        }

        return Result;
    }

    TDKernel(mathFunctionEvaluator *pEval, double dtA, size_t len)
    {
        Kernel = new QList <double>();
        Evaluator = pEval;
        dt = dtA;
        KernelLength = len;
        genKernel(Evaluator, dt, KernelLength);
    }

    QList<double> *getKernel(void)
    {
        return Kernel;
    }


private:
    mathFunctionEvaluator *Evaluator;
    QList<double> *Kernel;
    double dt;
    size_t KernelLength;
};

void putKernel(double *Kernel, int Length)
{
    for(int i = 0 ; i < Length ; i++ )
        printf("%i %f\n", i, *(Kernel+i));
}

void putKernel(QList <double> Kernel)
{
    for(int i = 0 ; i < Kernel.count() ; i++ )
        printf("%i %f\n", i, Kernel.at(i));
}

int main(int argc, char *argv[])
{
    mathFunctionCompiler *Compiler = new mathFunctionCompiler("1/pow((pow(s,2.0)+1/a*s+1), 2)", "s", "sorder");
    mathFunctionEvaluator *Eval = new mathFunctionEvaluator("s", "sorder");
    Eval->setVar("a", 1);

    int SignalLength = 10000;
    TDKernel Kernel1(Eval, 0.01, SignalLength);


    QList <double> Signal;


    double X = 1.0;
    double Y = 0;
    QList<double> Output;

    for( int i = 0 ; i < SignalLength ; i ++ )
    {
        Output.append(Y);
        double Error = (X - Y);
        Signal.append(0.9*Error);
        Y = Kernel1.convolvedDot(i, &Signal);


    }
    putKernel(Output);
}
