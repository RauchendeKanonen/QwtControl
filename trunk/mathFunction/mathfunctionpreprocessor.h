#ifndef MATHFUNCTIONPREPROCESSOR_H
#define MATHFUNCTIONPREPROCESSOR_H
#include "mpParser.h"
#include <QString>
#include <QPair>
#include <QList>
using namespace mup;

class mathFunctionpreprocessor
{
public:
    mathFunctionpreprocessor(QString Expression);
    bool preformatExponents(QString *Expression);
    bool preprocessExponents(QString *Expression);
    QString ExpressionString;

    QString preprocessedString(void);
    QString Message;
    bool state;
};

#endif // MATHFUNCTIONPREPROCESSOR_H
