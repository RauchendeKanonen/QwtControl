#include "mathfunctioncompiler.h"
#include <QFile>
#include <errno.h>

mathFunctionCompiler::mathFunctionCompiler(QString FunctionDefinition, QString IndependentVarName, QString FunctionName, bool Complex)
{
    if(Complex)
        compileComplex(FunctionDefinition, IndependentVarName, FunctionName);
    else
        compileReal(FunctionDefinition, IndependentVarName, FunctionName);
}

void mathFunctionCompiler::compileComplex(QString FunctionDefinition, QString IndependentVarName, QString FunctionName)
{
    QString SourceBuffer;
    QString HeaderBuffer;
    QString SourceFile("mathFunction/compile/");
    SourceFile.append(FunctionName+QString(".c"));

    QFile SourceF(SourceFile);
    SourceF.remove();

    QString HeaderFile("mathFunction/compile/");
    HeaderFile.append(FunctionName+QString(".h"));

    QFile SourceH(HeaderFile);
    SourceH.remove();

    QString ObjectFile("mathFunction/compile/");
    ObjectFile.append(FunctionName+QString(".o"));

    QFile ObjectF(ObjectFile);
    ObjectF.remove();

    QString OutputLib;
    OutputLib.append(QString("mathFunction/compile/") + FunctionName+QString(".so.0.1"));

    QStringList Parameters;
    Parameters.append(QString("complex <long double> ") + IndependentVarName);

    QFile LibF(OutputLib);
    LibF.remove();

    replaceComputerNotation(&FunctionDefinition);


    QStringList Variables = findCharacterStrings(FunctionDefinition);
    Variables.removeDuplicates();
    Variables.removeOne(IndependentVarName);

    castConstTo(&FunctionDefinition, QString("(complex <long double>)"));



    insertFunction(&SourceBuffer, FunctionName+QString("_complex"), Parameters, QString("extern \"C\" complex <long double> "));
    insertFunctionPrototype(&HeaderBuffer, FunctionName, Parameters, QString("extern \"C\" complex <long double> "));
    appendFunctionStatement(&SourceBuffer, FunctionName, QString("return ") + FunctionDefinition + QString(";"));

    Parameters.clear();
    Parameters.append(QString("void"));
    insertFunction(&SourceBuffer, QString("getVar"), Parameters, QString("extern \"C\" char** "));
    appendFunctionStatement(&SourceBuffer, QString("getVar"), QString("return ") + QString("VarNames") + QString(";"));



    QString VarList;
    VarList = QString("{");
    for(int i = 0 ; i < Variables.count() ; i ++ )
    {
        VarList += QString("\"") + Variables.at(i) + QString("\", ");
    }
    VarList += QString("NULL ");
    VarList += QString("}");

    insertGlobalVariable(&SourceBuffer,QString("char *VarNames[] = ") + VarList);


    Parameters.clear();
    Parameters.append(QString("char *Varname"));
    Parameters.append(QString("long double Value"));
    insertFunction(&SourceBuffer, QString("setVar"), Parameters, QString("extern \"C\" int "));
    insertFunctionPrototype(&HeaderBuffer, QString("setVar"), Parameters, QString("extern \"C\" int "));
    for(int i = 0 ; i < Variables.count() ; i ++ )
    {
        insertGlobalVariable(&SourceBuffer,QString("long double ") + Variables.at(i));
        QString If = ifStatement(QString("strcmp(Varname, ") + QString("\"") + Variables.at(i) + QString("\"") + QString(") == 0"));
        appendFunctionStatement(&SourceBuffer, QString("setVar"),If);
        appendFunctionStatement(&SourceBuffer, QString("setVar"),QString("       ") + Variables.at(i)  + QString(" = Value;"));
    }
    insertLocalIncludeStatement(&SourceBuffer, FunctionName+QString(".h"));
    insertLine(&HeaderBuffer, "using namespace std");
    insertLocalIncludeStatement(&HeaderBuffer, QString("complex"));
    insertLocalIncludeStatement(&HeaderBuffer, QString("math.h"));
    insertLocalIncludeStatement(&HeaderBuffer, QString("string.h"));

    WriteRet = writeFile(SourceFile, &SourceBuffer);
    WriteRet = writeFile(HeaderFile, &HeaderBuffer);

    CompileRet = compilegpp(SourceFile, ObjectFile);
    LinkRet = linkgpp(ObjectFile, OutputLib);
}

void mathFunctionCompiler::compileReal(QString FunctionDefinition, QString IndependentVarName, QString FunctionName)
{
    QString SourceBuffer;
    QString HeaderBuffer;
    QString SourceFile("mathFunction/compile/");
    SourceFile.append(FunctionName+QString(".c"));

    QFile SourceF(SourceFile);
    SourceF.remove();

    QString HeaderFile("mathFunction/compile/");
    HeaderFile.append(FunctionName+QString(".h"));

    QFile SourceH(HeaderFile);
    SourceH.remove();

    QString ObjectFile("mathFunction/compile/");
    ObjectFile.append(FunctionName+QString(".o"));

    QFile ObjectF(ObjectFile);
    ObjectF.remove();

    QString OutputLib;
    OutputLib.append(QString("mathFunction/compile/") + FunctionName+QString(".so.0.1"));

    QStringList Parameters;
    Parameters.append(QString("double ") + IndependentVarName);

    QFile LibF(OutputLib);
    LibF.remove();

    insertFunction(&SourceBuffer, FunctionName, Parameters, QString("double "));
    insertFunctionPrototype(&HeaderBuffer, FunctionName, Parameters, QString("double "));
    appendFunctionStatement(&SourceBuffer, FunctionName, QString("return ") + FunctionDefinition + QString(";"));

    Parameters.clear();
    Parameters.append(QString("void"));
    insertFunction(&SourceBuffer, QString("getVar"), Parameters, QString("char** "));
    appendFunctionStatement(&SourceBuffer, QString("getVar"), QString("return ") + QString("VarNames") + QString(";"));

    QStringList Variables = findCharacterStrings(FunctionDefinition);
    Variables.removeDuplicates();
    Variables.removeOne(IndependentVarName);
    Variables.removeOne("pow");

    QString VarList;
    VarList = QString("{");
    for(int i = 0 ; i < Variables.count() ; i ++ )
    {
        VarList += QString("\"") + Variables.at(i) + QString("\", ");
    }
    VarList += QString("NULL ");
    VarList += QString("}");

    insertGlobalVariable(&SourceBuffer,QString("char *VarNames[] = ") + VarList);


    Parameters.clear();
    Parameters.append(QString("char *Varname"));
    Parameters.append(QString("double Value"));
    insertFunction(&SourceBuffer, QString("setVar"), Parameters, QString("int "));
    insertFunctionPrototype(&HeaderBuffer, QString("setVar"), Parameters, QString("int "));
    for(int i = 0 ; i < Variables.count() ; i ++ )
    {
        insertGlobalVariable(&SourceBuffer,QString("double ") + Variables.at(i));
        QString If = ifStatement(QString("strcmp(Varname, ") + QString("\"") + Variables.at(i) + QString("\"") + QString(") == 0"));
        appendFunctionStatement(&SourceBuffer, QString("setVar"),If);
        appendFunctionStatement(&SourceBuffer, QString("setVar"),QString("       ") + Variables.at(i)  + QString(" = Value;"));
    }
    insertLocalIncludeStatement(&SourceBuffer, FunctionName+QString(".h"));

    insertLocalIncludeStatement(&HeaderBuffer, QString("math.h"));
    insertLocalIncludeStatement(&HeaderBuffer, QString("string.h"));

    WriteRet = writeFile(SourceFile, &SourceBuffer);
    WriteRet = writeFile(HeaderFile, &HeaderBuffer);

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

bool mathFunctionCompiler::linkgpp(QString Object, QString Dest)
{
    QString LinkerCommand;
    LinkerCommand.append(QString("g++ -shared -Wl,-soname,"));
    LinkerCommand.append(Dest);
    LinkerCommand.append(QString(" -o "));
    LinkerCommand.append(Dest);
    LinkerCommand.append(QString(" "));
    LinkerCommand.append(Object);

    if(0 == system(LinkerCommand.toStdString().c_str()))
        return true;

    return false;
}

bool mathFunctionCompiler::compilegpp(QString Source, QString Dest)
{
    QString CompilerCommand;
    CompilerCommand.append(QString("g++ "));
    CompilerCommand.append(QString(CXXFLAGS));
    CompilerCommand.append(QString(" "));
    CompilerCommand.append(Source + QString(" "));
    CompilerCommand.append(QString("-o "));
    CompilerCommand.append(Dest + QString(" "));

    if(0 == system(CompilerCommand.toStdString().c_str()))
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

void mathFunctionCompiler::insertFunctionPrototype(QString *SourceBuffer, QString FunctionName, QStringList Parameters, QString ReturnType)
{
    QString FunctionOutline;
    FunctionOutline.append(ReturnType);
    FunctionOutline.append(" ");
    FunctionOutline.append(FunctionName);
    FunctionOutline.append("(");
    if(Parameters.count())
        FunctionOutline.append(Parameters.at(0));
    for(int i = 1 ; i < Parameters.count() ; i ++ )
    {
        FunctionOutline.append(", ");
        FunctionOutline.append(Parameters.at(i));
    }
    FunctionOutline.append(");\n");
    SourceBuffer->append(FunctionOutline);
}
QString mathFunctionCompiler::ifStatement(QString Expression)
{
    return (QString("if(") + Expression + QString(")"));
}

void mathFunctionCompiler::insertFunction(QString *SourceBuffer, QString FunctionName, QStringList Parameters, QString ReturnType)
{
    QString FunctionOutline;
    FunctionOutline.append(ReturnType);
    FunctionOutline.append(" ");
    FunctionOutline.append(FunctionName);
    FunctionOutline.append("(");
    if(Parameters.count())
        FunctionOutline.append(Parameters.at(0));
    for(int i = 1 ; i < Parameters.count() ; i ++ )
    {
        FunctionOutline.append(", ");
        FunctionOutline.append(Parameters.at(i));
    }
    FunctionOutline.append(")\n");
    FunctionOutline.append("{\n");
    FunctionOutline.append("}\n");
    SourceBuffer->append(FunctionOutline);
}

void mathFunctionCompiler::insertLine(QString *SourceBuffer, QString LineDeclaration)
{
    SourceBuffer->insert(0, LineDeclaration + QString(";\n"));
}

void mathFunctionCompiler::insertGlobalVariable(QString *SourceBuffer, QString VariableDeclaration)
{
    SourceBuffer->insert(0, VariableDeclaration + QString(";\n"));
}

bool mathFunctionCompiler::appendFunctionStatement(QString *SourceBuffer, QString FunctionName, QString Statement)
{

    int index = SourceBuffer->indexOf(FunctionName, 0, Qt::CaseSensitive);
    if(index < 0)
        return false;
    index = SourceBuffer->indexOf(QString("}\n"), index, Qt::CaseSensitive);
    if(index < 0)
        return false;
    SourceBuffer->insert(index, QString("     ") + Statement+QString("\n"));
    return true;
}

bool mathFunctionCompiler::insertLocalIncludeStatement(QString *SourceBuffer, QString File)
{
    SourceBuffer->insert(0, QString("#include\"") + File + QString("\"\n"));
    return true;
}



bool mathFunctionCompiler::insertFunctionOld(QString *SourceBuffer, QString Function, QString VarName)
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
    Buffer->clear();

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
    {
        int i = errno;
        char *p = strerror(i);
        return false;
    }

    if(::write(OUTFILE, (char*)Buffer->toStdString().c_str(), Buffer->size()) == Buffer->size())
        ret = true;
    ::close(OUTFILE);

    return ret;
}
