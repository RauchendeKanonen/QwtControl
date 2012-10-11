#include "mathfunctioncompiler.h"


mathFunctionCompiler::mathFunctionCompiler(QString FunktionA, QString IndependentVarName, QString OutputLib)
{
    QString SourceBuffer;
    FunktionString = FunktionA;
    QString SourceFile("compile/template.c");
    QString ObjectFile("compile/template.o");

    ReadRet = readFile(QString("template/template.c"), &SourceBuffer);
    InsertRet = insertFunction(&SourceBuffer, FunktionString, IndependentVarName);
    WriteRet = writeFile(SourceFile, &SourceBuffer);

    CompileRet = compile(SourceFile, ObjectFile);
    LinkRet = link(ObjectFile, OutputLib);

}


bool mathFunctionCompiler::link(QString Object, QString Dest)
{
    QString LinkerCommand;
    LinkerCommand.append(QString("gcc -shared -Wl,-soname,"));
    LinkerCommand.append(Dest);
    LinkerCommand.append(QString(" -o "));
    LinkerCommand.append(Dest);
    LinkerCommand.append(QString(" "));
    LinkerCommand.append(Object);

    if(0 == system(LinkerCommand.toStdString().c_str()))
        return true;

    return false;
}

bool mathFunctionCompiler::compile(QString Source, QString Dest)
{
    QString CompilerCommand;
    CompilerCommand.append(QString("gcc "));
    CompilerCommand.append(QString(CFLAGS));
    CompilerCommand.append(QString("-c "));
    CompilerCommand.append(Source + QString(" "));
    CompilerCommand.append(QString("-o "));
    CompilerCommand.append(Dest);

    if(0 == system(CompilerCommand.toStdString().c_str()))
        return true;

    return false;
}

bool mathFunctionCompiler::insertFunction(QString *SourceBuffer, QString Function, QString VarName)
{
    int index;


    index = SourceBuffer->indexOf(QString("f()"));

    if(index)
    {
        SourceBuffer->insert(index+2, QString("double ") + VarName);
    }
    else
        return false;


    index = SourceBuffer->indexOf(QString("return "));

    if(index)
    {
        SourceBuffer->insert(index+7, Function);
    }

    else
        return false;

    index = SourceBuffer->size();
    QString IndepInfoFunction;
    IndepInfoFunction.append("char *indepVarName(void)\n");
    IndepInfoFunction.append("{\n");
    QString ReturnStatement;
    ReturnStatement.sprintf("  return \"%s\";\n", VarName.toStdString().c_str());
    IndepInfoFunction.append(ReturnStatement);
    IndepInfoFunction.append("}\n");
    SourceBuffer->append(IndepInfoFunction);
    return true;
}

bool mathFunctionCompiler::readFile(QString Path, QString *Buffer)
{
    int INFILE;
    bool ret = false;
    int size;
    char RawBuffer[1024*1024];

    if((INFILE = ::open(Path.toStdString().c_str(),O_RDONLY))<=0)
    {
        return false;
    }


    if((size = ::read(INFILE, (char*)RawBuffer, 1024*1024)) > 0)
    {
        Buffer->append(RawBuffer);
        ret = true;
    }
    return ret;
}

bool mathFunctionCompiler::writeFile(QString Path, QString *Buffer)
{
    int OUTFILE;
    bool ret = false;

    if((OUTFILE = ::open(Path.toStdString().c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO))<=0)
        return false;


    if(::write(OUTFILE, (char*)Buffer->toStdString().c_str(), Buffer->size()) == Buffer->size())
        ret = true;
    ::close(OUTFILE);

    return ret;
}
