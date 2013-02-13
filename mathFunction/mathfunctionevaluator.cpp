#include "mathfunctionevaluator.h"
#include "mathfunctioncompiler.h"
#include <complex>
#include <errno.h>
using namespace std;

mathFunctionEvaluator::mathFunctionEvaluator(QString IndepVarNameA, QString FunctionName)
{
    QString LibFile;
    LibFile = QString("mathFunction/compile/") + FunctionName + QString(".so.0.1");
    IndepVarName = IndepVarNameA;


    char libstr[1024];

    strcpy(libstr, LibFile.toStdString().c_str());
    libhandle = dlopen(libstr, RTLD_LAZY);


    dlerror();

    if(!libhandle)
    {
        char *est  = dlerror();
        printf(est);
        state = false;
        return;
    }

    state=true;
    realpf = (double (*)(double))dlsym(libhandle, FunctionName.toStdString().c_str());
    complexpf = (std::complex <long double>(*)(std::complex <long double>))dlsym(libhandle, (FunctionName+QString("_complex")).toStdString().c_str());
    char *est  = dlerror();
    printf(est);
    pSetVar = (void (*)(char *, long double))dlsym(libhandle, "setVar");
    pGetVar = (char** (*)(void))dlsym(libhandle, "getVar");
}

bool mathFunctionEvaluator::getState(void)
{
    return state;
}

QString mathFunctionEvaluator::indepVarName(void)
{
    return IndepVarName;
}

QStringList mathFunctionEvaluator::getExpressionVars(void)
{
    QStringList VarList;
    if(!state)
        return QStringList();
    char **Vars = pGetVar();
    for(; *Vars != NULL ; Vars ++)
    {
        VarList.append(*Vars);
    }
    return VarList;
}

void mathFunctionEvaluator::setVar(QString VarName, double Arg)
{
    if(state)
        pSetVar((char*)VarName.toStdString().c_str(), Arg);
}


double mathFunctionEvaluator::eval(double Arg)
{
    if(state && realpf)
        return realpf(Arg);
    return 0.0;
}

complex <long double> mathFunctionEvaluator::eval(complex <long double> Arg)
{
    if(state && complexpf)
        return complexpf(Arg);
}
