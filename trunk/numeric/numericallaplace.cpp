#include "numericallaplace.h"
#include <complex>



NumericalLaplace::NumericalLaplace(mathFunctionEvaluator *EvalA, int TransformTypeA)
{
    TransformType = TransformTypeA;
    Eval = EvalA;

    if(TRANSFORM_GAVER_STEHFEST == TransformType)
        InitStehfest(DefaultStehfestN);

    if(TRANSFORM_WEEKS == TransformType)
    {

    }
}

NumericalLaplace::~NumericalLaplace()
{

}

QPolygonF NumericalLaplace::InverseTransform(double dt, double tEnd)
{
    QPolygonF Output;

    if(TransformType == TRANSFORM_GAVER_STEHFEST)
    {
        for(double t = dt ; t <= tEnd ; t+=dt)
        {
            if(t <= 0.00001)
                t = 00001;
            long double ln2t = ln2 / t;

            long double x = 0;
            long double y = 0;

            for (int i = 0; i < V_Length; i++)
            {
                x += ln2t;

                std::complex <long double>arg(x, 0.0);


                y += V[i] * Eval->eval(arg).real();
            }
            Output.append(QPointF(t, ln2t * y));
        }
        return Output;
    }

    if(TransformType == TRANSFORM_WEEKS)
    {
        int m = tEnd/dt;
        int p = 9;
        int n = pow(2.0, p)-1;

        double t[m+1];
        double coeff[n];

        int i;

        for( i = 0 ; i <= m ; i ++ )
        {
            t[i] = dt*i;
        }

        /* (Fptr, NCoeff, convergence abzissica, convergence parm, auswetungsposition, num of points*/
        Output  = linvweex(Eval, p, 0.1, 5, 1.0, m, t, coeff);
        return Output;
    }
}



double NumericalLaplace::Factorial(int N)
{
    double x = 1;
    if (N > 1)
    {
        for (int i = 2; i <= N; i++)
            x = i * x;
    }
    return x;
}

void NumericalLaplace::InitStehfest(int N)
{
    ln2 = log(2.0);
    int N2 = N / 2;
    int NV = 2 * N2;
    V_Length = NV;
    V = new double[NV];
    int sign = 1;
    if ((N2 % 2) != 0)
        sign = -1;
    for (int i = 0; i < NV; i++)
    {
        int kmin = (i + 2) / 2;
        int kmax = i + 1;
        if (kmax > N2)
            kmax = N2;
        V[i] = 0;
        sign = -sign;
        for (int k = kmin; k <= kmax; k++)
        {
            V[i] = V[i] + (pow(k, N2) / Factorial(k)) * (Factorial(2 * k)
                / Factorial(2 * k - i - 1)) / Factorial(N2 - k) / Factorial(k - 1)
                / Factorial(i + 1 - k);
        }
        V[i] = sign * V[i];
    }
}
