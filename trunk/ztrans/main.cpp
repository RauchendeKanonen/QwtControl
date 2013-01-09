#include <QCoreApplication>
#include <ginac/ginac.h>

using namespace GiNaC;
using namespace std;

void test(ex Polynome, symbol s)
{

    ex Poly = Polynome.expand();

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i) {
        cout << "The x^" << i << "-coefficient is "
             << Poly.coeff(s,i) << endl;
    }
    cout << Poly;
}

int main(int argc, char *argv[])
{
    symbol x("x"), y("y"), z("z"), a("a"), b("b"), c("c"), d("d");

    ex Rational = (1+a*pow(z, -1)+b*pow(z,-2))/(1+c*pow(z, -1)+d*pow(z, -2));
    ex Numerator = Rational.numer();
    ex DeNumerator = Rational.denom();

    cout << Numerator  << endl;
    cout << DeNumerator  << endl;

    ex assembly = Numerator/DeNumerator;

    ex factor = Rational/assembly;
    factor = factor.expand();
    ex collect;
    collect.collect(factor);
    cout << collect << endl;
    //test(Numerator, z);
    //test(DeNumerator, z);
}


