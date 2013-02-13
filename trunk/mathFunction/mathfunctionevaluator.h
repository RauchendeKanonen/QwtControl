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
#include <complex>


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
    std::complex<long double> eval(std::complex<long double> Arg);
    bool getState(void);
private:
    void* libhandle;
    bool state;
    double(*realpf)(double);
    std::complex <long double>(*complexpf)(std::complex <long double>);
    void (*pSetVar)(char *, long double);
    char** (*pGetVar)(void);
    QString IndepVarName;
};

#endif // MATHFUNCTIONEVALUATOR_H
