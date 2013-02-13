#ifndef MATHFUNCTIONCOMPILER_H
#define MATHFUNCTIONCOMPILER_H
#include <QString>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <QStringList>
#include "util/tools.h"
#define CFLAGS "-fPIC -g "
#define CXXFLAGS "-c -pipe -g -Wall -W -D_REENTRANT -fPIC"

class mathFunctionCompiler
{
private:
    bool WriteRet;
    bool CompileRet;
    bool LinkRet;
public:
    bool getState(void)
    {
        if(WriteRet && CompileRet && LinkRet)
            return true;
        return false;
    }
    void insertGlobalVariable(QString *SourceBuffer, QString VariableDeclaration);
    void insertLine(QString *SourceBuffer, QString LineDeclaration);
    QString ifStatement(QString Expression);
    bool appendFunctionStatement(QString *SourceBuffer, QString FunctionName, QString Statement);
    void insertFunctionPrototype(QString *SourceBuffer, QString FunctionName, QStringList Parameters, QString ReturnType);
    bool compile(QString Source, QString Dest);
    bool compilegpp(QString Source, QString Dest);
    bool link(QString Object, QString Dest);
    bool linkgpp(QString Object, QString Dest);
    bool insertFunctionOld(QString *SourceBuffer, QString Function, QString VarName);
    void insertFunction(QString *SourceBuffer, QString FunctionName, QStringList Parameters, QString ReturnType);
    mathFunctionCompiler(QString FunctionDefinition, QString IndependentVarName, QString FunctionName, bool Complex=false);
    bool insertLocalIncludeStatement(QString *SourceBuffer, QString File);
    bool readFile(QString Path, QString *Buffer);
    bool writeFile(QString Path, QString *Buffer);
    void compileComplex(QString FunctionDefinition, QString IndependentVarName, QString FunctionName);
    void compileReal(QString FunctionDefinition, QString IndependentVarName, QString FunctionName);
};


#endif // MATHFUNCTIONCOMPILER_H
