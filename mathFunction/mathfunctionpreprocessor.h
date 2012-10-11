#ifndef MATHFUNCTIONPREPROCESSOR_H
#define MATHFUNCTIONPREPROCESSOR_H
#include "mpParser.h"
#include <QString>
#include <QPair>
#include <QList>
using namespace mup;

class mathFunctionpreprocessor
{
public:
    mathFunctionpreprocessor(ParserX Expression, QString IndepVarName);
    QList < QPair < QString, double > > getVariables(ParserX ExpressionX);
    QString ExpressionString;

    QString preprocessedString(void);
};

#endif // MATHFUNCTIONPREPROCESSOR_H
