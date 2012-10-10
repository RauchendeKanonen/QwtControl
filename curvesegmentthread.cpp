#include "curvesegmentthread.h"


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
    Restart:
    Value  InVal(CurveInfo->StartPoint);
    Variable InVar(&InVal);

    int i;
    int index = 0;

    //CurveInfo->Sem->acquire();
    try
    {
        if(CurveInfo->CurveType == CURVE_TYPE_XY_NUMERICAL)
        {

            int EndLoop = (CurveInfo->DataSize*(CurveInfo->Segment+1));

            for(i = CurveInfo->DataSize*CurveInfo->Segment ; i < EndLoop ; i ++)
            {
                if(!CurveInfo->Sem->tryAcquire())
                    goto Restart;
                CurveInfo->xData[index] = i*CurveInfo->Resolution;
                CurveInfo->yData[index] = Laplace.InverseTransform(CurveInfo->Expression, CurveInfo->xData[index]);

                index ++;
                CurveInfo->Sem->release();
            }

            if(CurveInfo->Segment == 0)
                CurveInfo->yData[0] = 0.0;
        }
    }
    catch(mup::ParserError &e)
    {
        std::cout << e.GetMsg() << std::endl;
    }
    //CurveInfo->Sem->release();
    emit CurveSegmentReady(CurveInfo);
}
