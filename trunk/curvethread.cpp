#include "curvethread.h"

CurveThread::CurveThread(QObject *parent, CurveInformationStruct *CurveInfoA) :
    QThread(parent)
{
    CurveInfo = CurveInfoA;
    start();
}


void CurveThread::run (void)
{
    Value  InVal(CurveInfo->StartPoint);
    Variable InVar(&InVal);

    CurveInfo->Sem->acquire();
    try
    {
        if(CurveInfo->CurveType == CURVE_TYPE_XY_COMPILED_NUMERICAL)
        {
            NumericalLaplace Laplace;

            QStringList VarList = CurveInfo->Evaluator->getExpressionVars();

            for(int i = 0 ; i < VarList.count() ; i ++ )
            {
                Value *Val = CurveInfo->pVariabelMdl->getVarValuePtr(VarList.at(i));
                double test = Val->GetFloat();
                CurveInfo->Evaluator->setVar(VarList.at(i), Val->GetFloat());
            }



            for(int i = 1 ; i < CurveInfo->DataSize ; i ++)
            {
                CurveInfo->xData[i] = i*CurveInfo->Resolution;
                CurveInfo->yData[i] = Laplace.InverseTransform(CurveInfo->Evaluator, i*CurveInfo->Resolution);
            }
            CurveInfo->Sem->release();
            emit CurveReady(CurveInfo);
            return;
        }

        CurveInfo->Expression->DefineVar(CurveInfo->IndepVarName, &InVar);
        Value  IncrementVal(CurveInfo->Resolution);
        Variable IncrementVar(&IncrementVal);

        for(int i = 0 ; i < CurveInfo->DataSize  ; i ++)
        {
            Value Result = CurveInfo->Expression->Eval();
            InVar += IncrementVar;

            if(CurveInfo->CurveType == CURVE_TYPE_XY)
            {
                CurveInfo->xData[i] = InVar.GetFloat();
                CurveInfo->yData[i] = Result.GetFloat();
            }
            else if(CurveInfo->CurveType == CURVE_TYPE_COMPLEX)
            {
                CurveInfo->xData[i] = Result.GetFloat();
                CurveInfo->yData[i] = Result.GetImag();
                double x = CurveInfo->xData[i];
                double y = CurveInfo->yData[i];
                x = y;
            }
            else if(CurveInfo->CurveType == CURVE_TYPE_MAGNITUDE)
            {
                CurveInfo->xData[i] = Result.GetFloat();
                CurveInfo->yData[i] = sqrt(pow(Result.GetImag(),2)+ pow(Result.GetFloat(),2 ));
            }
        }
        if(CurveInfo->CurveType == CURVE_TYPE_COMPLEX)
        {
           InVal = CurveInfo->MarkerPos;
           Value MarkerResult = CurveInfo->Expression->Eval();
           CurveInfo->Marker->setYValue(MarkerResult.GetImag());
           CurveInfo->Marker->setXValue(MarkerResult.GetFloat());
        }
    }
    catch(mup::ParserError &e)
    {
        std::cout << e.GetMsg() << std::endl;
    }
    CurveInfo->Sem->release();
    emit CurveReady(CurveInfo);
}
