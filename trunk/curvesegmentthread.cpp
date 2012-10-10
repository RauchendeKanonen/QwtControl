#include "curvesegmentthread.h"


CurveSegmentThread::CurveSegmentThread(QObject *parent, CurveInformationStruct *CurveInfoA) :
    QThread(parent)
{
    CurveInfo = CurveInfoA;
    start();
}


void CurveSegmentThread::run (void)
{
    Value  InVal(CurveInfo->StartPoint);
    Variable InVar(&InVal);

    int i;
    int index = 0;
    int m = 0;
    CurveInfo->Sem->acquire();
    try
    {
        if(CurveInfo->CurveType == CURVE_TYPE_XY_NUMERICAL)
        {
            NumericalLaplace Laplace;

            double LocalStartPoint = CurveInfo->DataSize*CurveInfo->Segment*CurveInfo->Resolution;

            for(i = CurveInfo->DataSize*CurveInfo->Segment ; i < (CurveInfo->DataSize*(CurveInfo->Segment+1)) ; i ++)
            {


                CurveInfo->xData[index] = i*CurveInfo->Resolution;
                CurveInfo->yData[index] = Laplace.InverseTransform(CurveInfo->Expression, i*CurveInfo->Resolution);

                if(fabs(CurveInfo->yData[index] - CurveInfo->yData[index-1]) > 10.0)
                    CurveInfo->yData[index] = CurveInfo->yData[index-1];
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
