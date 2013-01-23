#include "tools.h"


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
    Strings.removeOne(QString("atan2"));

    return Strings;
}
