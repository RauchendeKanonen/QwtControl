#include "controlexpression.h"
#include <QImage>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QStringList>
#include <math.h>
#include "util/tools.h"

QStringList ControlExpression::getVariables(void)
{
    return findCharacterStrings(ExpressionString);

}

mathFunctionEvaluator *ControlExpression::getComplexEvaluator(void)
{
    if(IndependentVarName == "")
        return NULL;
    mathFunctionCompiler Compiler(cSourceString(), IndependentVarName, ExpressionName+QString("complex"), true);
    bool comState = Compiler.getState();
    if(!comState)
    {
        return NULL;
    }
    mathFunctionEvaluator *Evaluator = new mathFunctionEvaluator(IndependentVarName, ExpressionName+QString("complex"));
    if(!Evaluator->getState())
    {
        delete Evaluator;
        return NULL;
    }
    return Evaluator;
}

mathFunctionEvaluator *ControlExpression::getRealEvaluator(void)
{
    if(IndependentVarName == "")
        return NULL;
    mathFunctionCompiler Compiler(cSourceStringRealPart(), IndependentVarName, ExpressionName+QString("real"));
    bool comState = Compiler.getState();
    if(!comState)
    {
        return NULL;
    }
    mathFunctionEvaluator *Evaluator = new mathFunctionEvaluator(IndependentVarName, ExpressionName+QString("real"));
    if(!Evaluator->getState())
    {
        delete Evaluator;
        return NULL;
    }
    return Evaluator;
}

mathFunctionEvaluator *ControlExpression::getImagEvaluator(void)
{
    if(IndependentVarName == "")
        return NULL;
    mathFunctionCompiler Compiler(cSourceStringImagPart(), IndependentVarName, ExpressionName+QString("imag"));
    bool comState = Compiler.getState();
    if(!comState)
    {
        return NULL;
    }
    mathFunctionEvaluator *Evaluator = new mathFunctionEvaluator(IndependentVarName, ExpressionName+QString("imag"));
    if(!Evaluator->getState())
    {
        delete Evaluator;
        return NULL;
    }
    return Evaluator;
}

void ControlExpression::setIndependentVarName(QString IndependentVarNameA)
{
    IndependentVarName = IndependentVarNameA;
}

QString ControlExpression::independentVarName(void)
{
    return IndependentVarName;
}
ControlExpression::~ControlExpression(void)
{
    if(exExpression)
        delete exExpression;
}

ControlExpression::ControlExpression(QString ExpressionDef, QString IndepVar)
{
    IndependentVarName = IndepVar;

    ExpressionName = getExpressionName(ExpressionDef);
    ExpressionString = getExpression(ExpressionDef);

    if(ExpressionName == QString(""))
        ExpressionName = QString("std");
    QString Random;
    Random.sprintf("_%d",rand());
    ExpressionName+=Random;
    QStringList Variables = findCharacterStrings(ExpressionString);
    Variables.removeDuplicates();
    GiNaC::lst VarList;
    state = true;
    for(int i = 0 ; i < Variables.count() ; i ++ )
    {
        GiNaC::realsymbol Sym(Variables.at(i).toStdString());
        VarList.append(Sym);
    }
    try
    {
        exExpression = new GiNaC::ex(ExpressionString.toStdString().c_str(), VarList);
    }
    catch (exception &p)
    {
        exExpression = NULL;
        state = false;
        cerr << p.what() << endl;
    }
}

QString ControlExpression::cSourceString(void)
{
    if(!state)
        return QString();

    ostringstream LatexOStream;
    try
    {
        LatexOStream << GiNaC::csrc_double;
        LatexOStream << *exExpression ;
    }
    catch (exception &p)
    {
        cerr << p.what() << endl;
    }
    QString Ret = QString::fromStdString(LatexOStream.str());
    return Ret;
}

QString ControlExpression::cSourceStringRealPart(void)
{
    if(!state)
        return QString();

    ostringstream LatexOStream;
    try
    {
        LatexOStream << GiNaC::csrc_double;
        LatexOStream << exExpression->real_part();
//        GiNaC::ex realEx = exExpression->real_part();
//        cout << exExpression->real_part() << endl;
//        for (size_t i = 0; i != realEx.nops(); ++i)
//        {
//            cout << realEx.op(i) << endl << endl;
//            for (size_t k = 0; k != realEx.op(i).nops() ; ++k)
//            {
//                cout << realEx.op(i).op(k) << endl << endl;
//            }
//
//        }
    }
    catch (exception &p)
    {
        cerr << p.what() << endl;
    }
    QString Ret = QString::fromStdString(LatexOStream.str());
    return Ret;
}

QString ControlExpression::cSourceStringImagPart(void)
{
    if(!state)
        return QString();

    ostringstream LatexOStream;
    try
    {
        LatexOStream << GiNaC::csrc_double;
        LatexOStream << exExpression->imag_part() ;
    }
    catch (exception &p)
    {
        cerr << p.what() << endl;
    }
    QString Ret = QString::fromStdString(LatexOStream.str());
    return Ret;
}

bool ControlExpression::getState(void)
{
    return state;
}

QString ControlExpression::latexString(void)
{
    if(!state)
        return QString();

    ostringstream LatexOStream;
    try
    {
        LatexOStream << GiNaC::latex;
        LatexOStream << *exExpression ;
    }
    catch (exception &p)
    {
        cerr << p.what() << endl;
    }
    QString Ret = QString::fromStdString(LatexOStream.str());
    return Ret;
}

QImage ControlExpression::picture(void)
{
    if(!state)
        return QImage("res/Error.png");

    QString LatexString = latexString();

    QString Filename = QString(ExpressionName) + QString(".tex");
    QString OutFilename = QString(ExpressionName) + QString(".png");

    QFile file(QString("latex/")+Filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return QImage();

    QTextStream out(&file);

    out << LatexString << "\n";


    QString ConverterCommand = QString("latex/tex2im");

    QStringList Args;
    Args << QString("-p") << QString("latex/")+Filename;

    QProcess Process;

    Process.setWorkingDirectory(Process.workingDirectory()+QString("latex/"));

    QString Wd = Process.workingDirectory();
    file.close();
    if(0 == Process.execute(ConverterCommand, Args))
    {
        return QImage(OutFilename);
    }

    return QImage();
}

QString ControlExpression::getExpressionName(void)
{
    return ExpressionName;
}

QString ControlExpression::getExpressionName(QString ExpressionDef)
{
    int FunctionNameEnd = ExpressionDef.indexOf(':');

    if(FunctionNameEnd > 0)
    {
        ExpressionDef = ExpressionDef.left(FunctionNameEnd);
        return ExpressionDef;
    }

    return QString();
}


QString ControlExpression::getExpression(QString ExpressionDef)
{
    int FunctionNameEnd = ExpressionDef.indexOf(':');

    if(FunctionNameEnd > 0)
        ExpressionDef = ExpressionDef.right(ExpressionDef.count() - FunctionNameEnd - 1);

    return ExpressionDef;
}

QString ControlExpression::getExpression(void)
{
    return ExpressionString;
}
