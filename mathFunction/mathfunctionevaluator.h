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



class mathFunctionEvaluator
{
public:
    void setVar(QString VarName, double Arg);
    mathFunctionEvaluator(QString IndepVarNameA, QString FunctionName);
    ~mathFunctionEvaluator(void)
    {
        dlclose(libhandle);
    }

    QString indepVarName(void);
    QStringList getExpressionVars(void);
    double eval(double Arg);
    bool getState(void);
private:
    void* libhandle;
    bool state;
    double(*pf)(double);
    void (*pSetVar)(char *, double);
    char** (*pGetVar)(void);
    QString IndepVarName;
};

#endif // MATHFUNCTIONEVALUATOR_H
