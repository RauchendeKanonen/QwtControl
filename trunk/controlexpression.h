#ifndef CONTROLEXPRESSION_H
#define CONTROLEXPRESSION_H
#include <QStringList>
#include <iostream>
#include <ostream>
#include <string>
#include <ginac/ginac.h>
#include <sstream>
#include <QImage>
#include "mathFunction/mathfunctionevaluator.h"
#include "mathFunction/mathfunctioncompiler.h"

using namespace std;


class ControlExpression
{
private:
    bool state;
    QString ExpressionName, ExpressionString;
    GiNaC::ex *exExpression;
    QString IndependentVarName;
    QStringList findCharacterStrings(QString Buffer);
public:
    QStringList getVariables(void);
    QString independentVarName(void);
    mathFunctionEvaluator *getRealEvaluator(void);
    mathFunctionEvaluator *getImagEvaluator(void);
    QString cSourceStringRealPart(void);
    QString cSourceStringImagPart(void);
    bool getState(void);
    QString cSourceString(void);
    QString getExpressionName(QString ExpressionDef);
    QString getExpressionName(void);
    QString getExpression(QString ExpressionDef);
    QString latexString(void);
    ControlExpression(QString ExpressionDef, QString IndepVar=QString());
    ~ControlExpression(void);
    QImage picture(void);
    void setIndependentVarName(QString IndependentVarNameA);
};

#endif // CONTROLEXPRESSION_H