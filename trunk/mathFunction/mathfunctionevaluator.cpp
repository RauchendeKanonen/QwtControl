#include "mathfunctionevaluator.h"
#include "mathfunctioncompiler.h"
#include "mathfunctionpreprocessor.h"



mathFunctionEvaluator::mathFunctionEvaluator(QString IndepVarNameA, QString FunctionName)
{
    QString LibFile;
    LibFile = QString("mathFunction/compile/") + FunctionName + QString(".so.0.1");
    IndepVarName = IndepVarNameA;


    char libstr[1024];
    char *ptr =  (char*)LibFile.toStdString().c_str();
    strcpy(libstr, ptr);
    void* libhandle = dlopen(libstr, RTLD_LAZY);




    if(!libhandle)
    {
        char *est  = dlerror();
        printf(est);
        state = false;
        return;
    }

    state=true;
    pf = (double (*)(double))dlsym(libhandle, FunctionName.toStdString().c_str());
    pSetVar = (void (*)(char *, double))dlsym(libhandle, "setVar");
    pGetVar = (char** (*)(void))dlsym(libhandle, "getVar");
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
    if(state)
        return pf(Arg);
    return 0.0;
}

