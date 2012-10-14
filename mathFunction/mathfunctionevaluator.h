#ifndef MATHFUNCTIONEVALUATOR_H
#define MATHFUNCTIONEVALUATOR_H

#include <QString>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <dlfcn.h>
#include "mpParser.h"

using namespace mup;


class mathFunctionEvaluator
{
public:
    void setVar(QString VarName, double Arg);
    mathFunctionEvaluator(QString IndepVarNameA, QString FunctionName);
    QString indepVarName(void);
    QStringList getExpressionVars(void);
    double eval(double Arg);
private:
    bool state;
    double(*pf)(double);
    void (*pSetVar)(char *, double);
    char** (*pGetVar)(void);
    QString IndepVarName;
};

#endif // MATHFUNCTIONEVALUATOR_H
