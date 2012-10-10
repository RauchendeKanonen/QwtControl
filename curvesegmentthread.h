#ifndef CURVESEGMENTTHREAD_H
#define CURVESEGMENTTHREAD_H



#include <QThread>
#include "mpParser.h"
#include "numericallaplace.h"
#include "definitions.h"

using namespace mup;

class CurveSegmentThread : public QThread
{
    Q_OBJECT
public:
    explicit CurveSegmentThread(QObject *parent, CurveInformationStruct *CurveInfoA);
    ~CurveSegmentThread(void);
    void run (void);
signals:
    void CurveSegmentReady(CurveInformationStruct *CurveInfo);
public slots:
private:
    NumericalLaplace Laplace;
    CurveInformationStruct *CurveInfo;
};

#endif // CURVESEGMENTTHREAD_H
