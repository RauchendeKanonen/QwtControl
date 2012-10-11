#include "curvesegmentthread.h"
#include "mathFunction/mathfunctionevaluator.h"


CurveSegmentThread::CurveSegmentThread(QObject *parent, CurveInformationStruct *CurveInfoA) :
    QThread(parent)
{
    CurveInfo = CurveInfoA;
    start();
}
CurveSegmentThread::~CurveSegmentThread(void)
{

}

void CurveSegmentThread::run (void)
{

    //mathFunctionEvaluator Evaluator(*CurveInfo->Expression, QString("s"), QString("Funktion1"));



    Restart:
    Value  InVal(CurveInfo->StartPoint);
    Variable InVar(&InVal);

    int i;
    int index = 0;

    CurveInfo->Sem->acquire();
    try
    {
        if(CurveInfo->CurveType == CURVE_TYPE_XY_NUMERICAL)
        {

            int EndLoop = (CurveInfo->DataSize*(CurveInfo->Segment+1));

            for(i = CurveInfo->DataSize*CurveInfo->Segment ; i < EndLoop ; i ++)
            {
                CurveInfo->xData[index] = i*CurveInfo->Resolution;
                CurveInfo->yData[index] = Laplace.InverseTransform(CurveInfo->Expression, CurveInfo->xData[index]);
                //CurveInfo->yData[index] = Laplace.InverseTransform(&Evaluator, CurveInfo->xData[index]);
                index ++;

            }

            if(CurveInfo->Segment == 0)
                CurveInfo->yData[0] = 0.0;
        }
    }
    catch(mup::ParserError &e)
    {
        std::cout << e.GetMsg() << std::endl;
    }
    CurveInfo->Sem->release();
    emit CurveSegmentReady(CurveInfo);
}
