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
        NumCoeffWeeks = 9;
        ConvergenceAbscissaWeeks = 0.1;
        ContourScaleWeeks = 1.0;
        EvaluationPosWeeks = 1.0;
    }
}

NumericalLaplace::~NumericalLaplace()
{

}

void NumericalLaplace::setup(void)
{
    if(TransformType == TRANSFORM_GAVER_STEHFEST)
    {

    }
    if(TransformType == TRANSFORM_WEEKS)
    {
        WeekSetupDialog Dlg(NumCoeffWeeks, ConvergenceAbscissaWeeks, ContourScaleWeeks, EvaluationPosWeeks );
        Dlg.setModal(true);
        if(Dlg.exec())
            Dlg.getParms(&NumCoeffWeeks, &ConvergenceAbscissaWeeks, &ContourScaleWeeks, &EvaluationPosWeeks);
    }
}

void NumericalLaplace::setup(NumericalLaplace *pLaplace)
{
    NumCoeffWeeks = pLaplace->NumCoeffWeeks;
    ConvergenceAbscissaWeeks = pLaplace->ConvergenceAbscissaWeeks;
    ContourScaleWeeks = pLaplace->ContourScaleWeeks;
    EvaluationPosWeeks = pLaplace->EvaluationPosWeeks;
}


QPolygonF NumericalLaplace::InverseTransform(double dt, double tEnd, double tStart)
{
    ThreadOutput.clear();
    if(TransformType == TRANSFORM_GAVER_STEHFEST)
    {
        for(double t = tStart ; t <= tEnd ; t+=dt)
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
            ThreadOutput.append(QPointF(t, ln2t * y));
        }
        return ThreadOutput;
    }

    if(TransformType == TRANSFORM_WEEKS)
    {
        int m = (tEnd-tStart)/dt;
        int p = NumCoeffWeeks;
        int n = pow(2.0, p)-1;

        double t[m+1];
        double coeff[n];

        int i;

        for( i = 0 ; i <= m ; i ++ )
        {
            t[i] = tStart+dt*i;
        }

        /* (Fptr, NCoeff, convergence abzissica, contour scale parm, auswetungsposition, num of points*/
        linvweex(Eval, p, ConvergenceAbscissaWeeks, ContourScaleWeeks, EvaluationPosWeeks, m, t, coeff);
        return ThreadOutput;
    }
    return ThreadOutput;
}


void NumericalLaplace::InverseTransformSetup(double dt, double tEnd, double tStart)
{
    ThreadDt = dt;
    ThreadtEnd = tEnd;
    ThreadtStart = tStart;
    start();
}


void NumericalLaplace::run(void)
{
    ThreadOutput.clear();
    if(TransformType == TRANSFORM_WEEKS)
    {
        int m = (ThreadtEnd-ThreadtStart)/ThreadDt;
        int p = NumCoeffWeeks;
        int n = pow(2.0, p)-1;

        double t[m+1];
        double coeff[n];

        int i;

        for( i = 0 ; i <= m ; i ++ )
        {
            t[i] = ThreadtStart+ThreadDt*i;
        }

        /* (Fptr, NCoeff, convergence abzissica, contour scale parm, auswetungsposition, num of points*/
        linvweex(Eval, p, ConvergenceAbscissaWeeks, ContourScaleWeeks, EvaluationPosWeeks, m, t, coeff);
    }
}

