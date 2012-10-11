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
    mathFunctionEvaluator(ParserX Parser, QString IndepVarName, QString FunctionName);
    QString indepVarName(void);
    double eval(double Arg);
private:
    bool state;
    double(*pf)(double);
    char *(*pIndepVarName)(void);
};

#endif // MATHFUNCTIONEVALUATOR_H
