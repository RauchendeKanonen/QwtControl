#include "tools.h"
#include <math.h>
#include <QDialog>
#include <iostream>

#include <iostream>
#include <string>
#include <ginac/ginac.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "controlexpression.h"
#include <QMessageBox>




GiNaC::ex EXinverseZTransform(GiNaC::ex Rational, GiNaC::symbol s)
{
    GiNaC::symbol x("x");
    GiNaC::symbol y("y");
    GiNaC::symbol *y0;
    GiNaC::ex Numerator = Rational.numer();
    GiNaC::ex DeNumerator = Rational.denom();

    GiNaC::ex Polynome = DeNumerator;

    Polynome /= GiNaC::ex(pow(s, Polynome.expand().degree(s)));
    GiNaC::ex Poly = Polynome.expand();
    GiNaC::ex YTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;
        if(i!=0)
            SymbolStr.sprintf("%s[i%d]",y.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s[i]",y.get_name().c_str());

        GiNaC::symbol *ydq = new GiNaC::symbol(SymbolStr.toStdString());
        if(i==0)
            y0 = ydq;
        YTerm += *ydq*Poly.coeff(s,i);
    }


    Polynome = Numerator;
    Polynome /= GiNaC::ex(pow(s, Polynome.expand().degree(s)));
    Poly = Polynome.expand();
    GiNaC::ex XTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;


        if(i!=0)
            SymbolStr.sprintf("%s[i%d]",x.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s[i]",x.get_name().c_str());

        GiNaC::symbol *xdq = new GiNaC::symbol(SymbolStr.toStdString());
        XTerm += *xdq*Poly.coeff(s,i);
    }

    GiNaC::ex DifferenceEquation = YTerm==XTerm;

    DifferenceEquation = GiNaC::lsolve(DifferenceEquation, *y0);
    return DifferenceEquation;
}

QString inverseZTransform(QString ZDExpression)
{
    GiNaC::parser reader;
    GiNaC::ex Expression;

    try
    {
        Expression = reader(ZDExpression.toStdString());
    }
    catch (exception &p)
    {
        QMessageBox Box;
        Box.setText(p.what());
        Box.setModal(true);
        Box.exec();
        return QString();
    }

    GiNaC::symtab table = reader.get_syms();
    GiNaC::symbol z = table.find("z") != table.end() ?
                GiNaC::ex_to<GiNaC::symbol>(table["z"]) : GiNaC::symbol("z");
    GiNaC::ex DifferenceEq = EXinverseZTransform(Expression, z);

    ostringstream DiffStr;
    DiffStr << DifferenceEq;
    return QString::fromStdString(DiffStr.str());
}


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
            for(; (i < Buffer->size()) && (Buffer->at(i).isDigit() || Buffer->at(i) == QChar('.')) ; i++);
            Buffer->insert(i, ')');
            continue;
        }
        if(Buffer->at(i).isDigit() && !Buffer->at(i-1).isDigit() && Buffer->at(i-1) != QChar('.'))
        {
            Buffer->insert(i, '(');
            i++;
            Buffer->insert(i, Cast);
            i+=Cast.count();
            for(; (i < Buffer->size()) && (Buffer->at(i).isDigit() || Buffer->at(i) == QChar('.')) ; i++);
            Buffer->insert(i, ')');
        }
    }
}

void replaceComputerNotation(QString *Buffer)
{
    for(int i = 0 ; i < Buffer->count() ; i ++)
    {
        if(i < Buffer->count()-1 && i > 0 && Buffer->at(i) == 'e' && Buffer->at(i+1) == '+' && Buffer->at(i-1).isDigit())
        {
            double Multiplier = 10.0;
            if(Buffer->at(i+2).isDigit())
                Multiplier = pow(10, strtof(Buffer->toStdString().c_str()+i+1, NULL));
            Buffer->replace(i, 1, '*');
            Buffer->remove(i+1, 1);

            while(Buffer->count() > i+1 && Buffer->at(i+1).isDigit())
                Buffer->remove(i+1, 1);

            QString MulStr;
            MulStr.sprintf("%g", Multiplier);

            Buffer->insert(i+1, MulStr);
        }

        if(i > 0 && Buffer->at(i) == 'e' && Buffer->at(i+1) == '-' && Buffer->at(i-1).isDigit())
        {
            double Multiplier = 10.0;
            if(Buffer->at(i+2).isDigit())
                Multiplier = pow(10,strtof(Buffer->toStdString().c_str()+i+1, NULL));
            Buffer->replace(i, 1, '*');
            Buffer->remove(i+1, 1);

            while(Buffer->at(i+1).isDigit())
                Buffer->remove(i+1, 1);


            QString MulStr;
            MulStr.sprintf("%g", Multiplier);


            Buffer->insert(i+1, MulStr);
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
