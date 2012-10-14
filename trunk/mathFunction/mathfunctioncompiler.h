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
#define CFLAGS "-fPIC -g "


class mathFunctionCompiler
{
private:
    bool InsertRet;
    bool WriteRet;
    bool ReadRet;
    bool CompileRet;
    bool LinkRet;
public:
    bool getState(void)
    {
        if(InsertRet && WriteRet && ReadRet && CompileRet && LinkRet)
            return true;
        return false;
    }
    void insertGlobalVariable(QString *SourceBuffer, QString VariableDeclaration);
    QString ifStatement(QString Expression);
    QStringList findCharacterStrings(QString Buffer);
    bool appendFunctionStatement(QString *SourceBuffer, QString FunctionName, QString Statement);
    void insertFunctionPrototype(QString *SourceBuffer, QString FunctionName, QStringList Parameters, QString ReturnType);
    bool compile(QString Source, QString Dest);
    bool link(QString Object, QString Dest);
    bool insertFunctionOld(QString *SourceBuffer, QString Function, QString VarName);
    void insertFunction(QString *SourceBuffer, QString FunctionName, QStringList Parameters, QString ReturnType);
    mathFunctionCompiler(QString FunctionDefinition, QString IndependentVarName, QString FunctionName);
    bool insertLocalIncludeStatement(QString *SourceBuffer, QString File);
    bool readFile(QString Path, QString *Buffer);
    bool writeFile(QString Path, QString *Buffer);
};


#endif // MATHFUNCTIONCOMPILER_H
