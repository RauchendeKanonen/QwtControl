#include <QCoreApplication>
#include "mathFunction/mathfunctioncompiler.h"
#include "mathFunction/mathfunctionevaluator.h"
#include "numericallaplace.h"
#include "mathFunction/controlexpression.h"
#include "rtresponcewindow.h"
#include <QtGui/QApplication>

class TDKernel
{
public:
    bool genKernel(mathFunctionEvaluator *Eval, double dt, int Length)
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

    double convolvedDot(size_t n, QList<double> *Signal)
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

    TDKernel(mathFunctionEvaluator *pEval, double dtA, size_t len)
    {
        Evaluator = pEval;
        dt = dtA;
        KernelLength = len;
        genKernel(Evaluator, dt, KernelLength);
    }

    QPolygonF getKernel(void)
    {
        return Kernel;
    }


private:
    mathFunctionEvaluator *Evaluator;
    QPolygonF Kernel;
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

void putKernel(QPolygonF Kernel)
{
    for(int i = 0 ; i < Kernel.count() ; i++ )
        printf("%i %f\n", i, Kernel.at(i).y());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlExpression *Expression = new ControlExpression("1/pow((pow(s,2.0)+1/a*s+1), 2)", "s");

    mathFunctionEvaluator *Eval = Expression->getComplexEvaluator();
    Eval->setVar("a", 1);

    int SignalLength = 10000;
    TDKernel Kernel1(Eval, 0.01, SignalLength);


    QList <double> Signal;


    double X = 1.0;
    double Y = 0;
    QList<double> Output;

    RTResponceWindow Dlg;

    Dlg.show();
    for( int i = 0 ; i < SignalLength*2 ; i ++ )
    {
        Output.append(Y);
        double Error = (X - Y);
        Signal.append(0.9*Error);
        Y = Kernel1.convolvedDot(i, &Signal);
    }
    putKernel(Output);
}
