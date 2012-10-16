#ifndef CONTROLEXPRESSION_H
#define CONTROLEXPRESSION_H
#include <QStringList>
#include <iostream>
#include <ostream>
#include <string>
#include <ginac/ginac.h>
#include <sstream>
#include <QImage>
using namespace std;
using namespace GiNaC;

class ControlExpression
{
private:
    bool state;
    QString ExpressionName, ExpressionString;
    ex *exExpression;
    QStringList findCharacterStrings(QString Buffer);
public:
    bool getState(void);
    QString cSourceString(void);
    QString getExpressionName(QString ExpressionDef);
    QString getExpression(QString ExpressionDef);
    QString latexString(void);
    ControlExpression(QString ExpressionDef);
    QImage picture(void);
};

#endif // CONTROLEXPRESSION_H
