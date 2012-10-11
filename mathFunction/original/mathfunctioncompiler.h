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

#define CFLAGS "-fPIC -O3 "


class mathFunctionCompiler
{
private:
    QString FunktionString;
    bool InsertRet;
    bool WriteRet;
    bool ReadRet;
    bool CompileRet;
    bool LinkRet;
public:
    bool getState(void)
    {
        if(InsertRet && WriteRet && ReadRet && CompileRet)
            return true;
        return false;
    }
    bool compile(QString Source, QString Dest);
    bool link(QString Object, QString Dest);
    bool insertFunction(QString *SourceBuffer, QString Function, QString VarName);
    mathFunctionCompiler(QString Funktion, QString IndependentVarName, QString OutputLib);
    bool readFile(QString Path, QString *Buffer);
    bool writeFile(QString Path, QString *Buffer);
};


#endif // MATHFUNCTIONCOMPILER_H
