#include <QCoreApplication>
#include <ginac/ginac.h>

using namespace GiNaC;
using namespace std;

ex assembleDifferenceEquation(ex Rational, symbol s)
{
    symbol x("x");
    symbol y("y");
    symbol *y0;
    ex Numerator = Rational.numer();
    ex DeNumerator = Rational.denom();

    ex Polynome = DeNumerator;

    Polynome /= ex(pow(s, Polynome.expand().degree(s)));
    ex Poly = Polynome.expand();
    ex YTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;
        if(i!=0)
            SymbolStr.sprintf("%s(i%d)",y.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s(i)",y.get_name().c_str());

        symbol *ydq = new symbol(SymbolStr.toStdString());
        if(i==0)
            y0 = ydq;
        YTerm += *ydq*Poly.coeff(s,i);
    }


    Polynome = Numerator;
    Polynome /= ex(pow(s, Polynome.expand().degree(s)));
    Poly = Polynome.expand();
    ex XTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;


        if(i!=0)
            SymbolStr.sprintf("%s(i%d)",x.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s(i)",x.get_name().c_str());

        symbol *xdq = new symbol(SymbolStr.toStdString());
        XTerm += *xdq*Poly.coeff(s,i);
    }

    ex DifferenceEquation = YTerm==XTerm;

    DifferenceEquation = lsolve(DifferenceEquation, *y0);
    return DifferenceEquation;
}

void reorderEquation(ex Polynome, symbol s)
{

}


int main(int argc, char *argv[])
{

    symbol x("x"), y("y"), z("z"), a("a"), b("b"), c("c"), d("d");

    ex Rational = (1+a*pow(z, -1)+b*pow(z,-2))/(1+c*pow(z, -1)+d*pow(z, -2));



    ex DifferenceEquation = assembleDifferenceEquation(Rational, z);
    cout << DifferenceEquation << endl;
    //DifferenceEquation = lsolve(DifferenceEquation, y_next);
}


