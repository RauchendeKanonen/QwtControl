#ifndef CURVETHREAD_H
#define CURVETHREAD_H

#include <QThread>
#include "mpParser.h"
#include "numericallaplace.h"
#include "definitions.h"

using namespace mup;

class CurveThread : public QThread
{
    Q_OBJECT
public:
    explicit CurveThread(QObject *parent, CurveInformationStruct *CurveInfoA);
    void run (void);
signals:
    void CurveReady(CurveInformationStruct *CurveInfo);
public slots:
private:
    CurveInformationStruct *CurveInfo;
};

#endif // CURVETHREAD_H
