#include "controlexpression.h"
#include <QImage>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QStringList>
QStringList ControlExpression::findCharacterStrings(QString Buffer)
{
    QStringList Strings;
    QString ActString;
    for(int i = 0 ; i < Buffer.count() ; i ++)
    {
        if(Buffer.at(i).isLetter())
            ActString.append(Buffer.at(i));
        else if(ActString.count())
        {
            Strings.append(ActString);
            ActString.clear();
        }
    }
    if(ActString.count())
        Strings.append(ActString);
    return Strings;
}


ControlExpression::ControlExpression(QString ExpressionDef)
{
    ExpressionName = getExpressionName(ExpressionDef);
    ExpressionString = getExpression(ExpressionDef);
    if(ExpressionName == QString(""))
        ExpressionName = QString("std");
    QStringList Variables = findCharacterStrings(ExpressionString);
    Variables.removeDuplicates();
    lst VarList;
    state = true;
    for(int i = 0 ; i < Variables.count() ; i ++ )
    {
        symbol Sym(Variables.at(i).toStdString());
        VarList.append(Sym);
    }
    try
    {
        exExpression = new ex(ExpressionString.toStdString().c_str(), VarList);
    }
    catch (exception &p)
    {
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
        LatexOStream << csrc_double;
        LatexOStream << *exExpression ;
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
        LatexOStream << latex;
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
