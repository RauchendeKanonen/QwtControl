#include <QCoreApplication>
#include <ginac/ginac.h>

using namespace GiNaC;
using namespace std;

ex assembleDifferenceEquation(ex Polynome, symbol s, symbol DiffS)
{
    ex DifferenceEquation;
    Polynome /= ex(pow(s, Polynome.expand().degree(s)));
    ex Poly = Polynome.expand();
    cout << Poly << endl;
    ex Term;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        /* QString TermStr;

        if(i!=0)
            TermStr.sprintf("%s[i%d]", EvS.get_name().c_str(), i);
        else
            TermStr.sprintf("%s[i]", EvS.get_name().c_str());

        cout << TermStr.toStdString() << endl;*/
        QString SymbolStr;


        if(i!=0)
            SymbolStr.sprintf("%s[i%d]",DiffS.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s[i]",DiffS.get_name().c_str());

        symbol y(SymbolStr.toStdString());
        Term += y*Poly.coeff(s,i)/DiffS;

        cout << "The " << s << "^" << i << "-coefficient is "
             << Poly.coeff(s,i) << endl;
    }
    cout << Term << endl;
    return Term;
}

void reorderEquation(ex Polynome, symbol s)
{

}


int main(int argc, char *argv[])
{

    symbol x("x"), y("y"), z("z"), a("a"), b("b"), c("c"), d("d");

    ex Rational = (1+a*pow(z, -1)+b*pow(z,-2))/(1+c*pow(z, -1)+d*pow(z, -2));
    ex Numerator = Rational.numer();
    ex DeNumerator = Rational.denom();

    ex left = y*DeNumerator;
    ex right = x*Numerator;

    ex Equation = left == right;


    left = assembleDifferenceEquation(left, z, y);
    right = assembleDifferenceEquation(right, z, x);

    symbol y_next("y[i]");

    ex DifferenceEquation = left == right;
    //DifferenceEquation = lsolve(DifferenceEquation, y_next);
    cout << endl << DifferenceEquation << endl;
}


