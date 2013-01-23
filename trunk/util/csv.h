#ifndef CSV_H
#define CSV_H
#include <QString>
#include <QVector>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <QPolygonF>

#include <sys/types.h>
#include <sys/stat.h>

class csv
{
private:
    QPolygonF Curve;
    QString Path;
    char delim;
public:
    void setDelimiter(char delimA);
    csv(QString PathArg);
    int readCSV(int numRows);
    int writeCSV(void);
    void setData(QPolygonF *CurveArg);
    QPolygonF getData(void);
};

#endif // CSV_H
