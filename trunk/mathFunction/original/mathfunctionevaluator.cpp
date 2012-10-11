#include "mathfunctionevaluator.h"
#include "mathfunctioncompiler.h"
#include "mathfunctionpreprocessor.h"



mathFunctionEvaluator::mathFunctionEvaluator(ParserX Parser, QString IndepVarName, QString FunctionName)
{
    QString LibFile;
    LibFile = QString("compile/") + FunctionName + QString(".so.1.0");

    mathFunctionpreprocessor Preprocessor(Parser, IndepVarName);
    QString PreprocessedString = Preprocessor.preprocessedString();



    printf("Compiling %s\n", PreprocessedString.toStdString().c_str());
    mathFunctionCompiler *Compiler = new mathFunctionCompiler(PreprocessedString, IndepVarName, LibFile);
    bool ret = Compiler->getState();
    delete Compiler;
    printf("Compilation Successful!\n");
    printf("expression with %d chars\n", PreprocessedString.size());

    if(!ret)
    {
        state = false;
        return;
    }


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


    pf = (double (*)(double))dlsym(libhandle, "f");
    pIndepVarName = (char *(*)(void))dlsym(libhandle, "IndepVarName");
}


double mathFunctionEvaluator::eval(double Arg)
{
    return pf(Arg);
}

QString mathFunctionEvaluator::indepVarName(void)
{
    return QString(pIndepVarName());
}
