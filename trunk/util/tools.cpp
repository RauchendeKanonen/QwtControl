#include "tools.h"

void toComplexExpression(QString *Buffer)
{
    Buffer->replace(QString("sin"),QString("csin"));
    Buffer->replace(QString("cos"),QString("ccos"));
    Buffer->replace(QString("sinh"),QString("csinh"));
    Buffer->replace(QString("cosh"),QString("ccosh"));
    Buffer->replace(QString("tanh"), QString("ctanh"));
    Buffer->replace(QString("atanh"),QString("catanh"));
    Buffer->replace(QString("sqrt"),QString("csqrt"));
    Buffer->replace(QString("log"),QString("clog"));
    Buffer->replace(QString("log2"),QString("clog2"));
    Buffer->replace(QString("sign"),QString("csign"));
    Buffer->replace(QString("logb"),QString("clogb"));
    Buffer->replace(QString("log10"),QString("clog10"));
    Buffer->replace(QString("atan"),QString("catan"));
    Buffer->replace(QString("exp"),QString("cexp"));
    Buffer->replace(QString("exp2"),QString("cexp2"));
    Buffer->replace(QString("acos"),QString("cacos"));
    Buffer->replace(QString("asin"),QString("casin"));
    Buffer->replace(QString("tan"),QString("ctan"));
    Buffer->replace(QString("fabs"),QString("cabs"));
    Buffer->replace(QString("pow"),QString("cpow"));
    Buffer->replace(QString("atan2"),QString("catan2"));
}

void castConstTo(QString *Buffer, QString Cast)
{
    for(int i = 0 ; i < Buffer->count() ; i ++)
    {
        if(i == 0 && Buffer->at(0).isDigit())
        {
            Buffer->insert(i, '(');
            i++;
            Buffer->insert(i, Cast);
            i+=Cast.count()+1;
            for(;Buffer->at(i).isDigit() || Buffer->at(i) == QChar('.'); i++);
            Buffer->insert(i, ')');
            continue;
        }
        if(Buffer->at(i).isDigit() && !Buffer->at(i-1).isDigit() && Buffer->at(i-1) != QChar('.'))
        {
            Buffer->insert(i, '(');
            i++;
            Buffer->insert(i, Cast);
            i+=Cast.count();
            for(;Buffer->at(i).isDigit() || Buffer->at(i) == QChar('.'); i++);
            Buffer->insert(i, ')');
        }

    }
}

QStringList findCharacterStrings(QString Buffer)
{
    QStringList Strings;
    QString ActString;
    for(int i = 0 ; i < Buffer.count() ; i ++)
    {
        if(Buffer.at(i).isLetter())
            ActString.append(Buffer.at(i));
        else if(ActString.count())
        {
            if(ActString == QString("e"))
            {
                if(i > 1 && Buffer.at(i-2).isDigit())
                {
                    ActString.clear();
                    continue;
                }

            }
            Strings.append(ActString);
            ActString.clear();
        }
    }
    if(ActString.count())
        Strings.append(ActString);
    Strings.removeDuplicates();
    Strings.removeOne(QString("sin"));
    Strings.removeOne(QString("cos"));
    Strings.removeOne(QString("sinh"));
    Strings.removeOne(QString("cosh"));
    Strings.removeOne(QString("tanh"));
    Strings.removeOne(QString("atanh"));
    Strings.removeOne(QString("sqrt"));
    Strings.removeOne(QString("log"));
    Strings.removeOne(QString("log2"));
    Strings.removeOne(QString("sign"));
    Strings.removeOne(QString("logb"));
    Strings.removeOne(QString("log10"));
    Strings.removeOne(QString("atan"));
    Strings.removeOne(QString("exp"));
    Strings.removeOne(QString("exp2"));
    Strings.removeOne(QString("acos"));
    Strings.removeOne(QString("asin"));
    Strings.removeOne(QString("tan"));
    Strings.removeOne(QString("cotan"));
    Strings.removeOne(QString("fabs"));
    Strings.removeOne(QString("pow"));
    Strings.removeOne(QString("std"));
    Strings.removeOne(QString("complex"));
    Strings.removeOne(QString("double"));
    Strings.removeOne(QString("float"));
    Strings.removeOne(QString("atan2"));
    return Strings;
}
