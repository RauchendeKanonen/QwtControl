#include <numericallaplace.h>

#include <math.h>
#include <complex>
using namespace std;
/* --------------------------------------------------------------------

            DOKUMENTATION

Funktion:   xfft

Zweck:      xfft  berechnet nach dem Cooley-Tukey-Algorithmus

            a)
              die diskrete Fourier-Transformation des komplexen
              Vektors w = u + i*v,
              u = (u[0],...,u[n-1]), v = (v[0],...,v[n-1]),
              also
                            n-1
              c[j] = 1/n * summe  w[k]*exp(-2*pi*i*j*k/n), j= 0,...,n-1
                            k=0

              oder

            b)
              die diskrete inverse Fourier-Transformation, also
              aus dem komplexen Vektor c

                       n-1
              w[k] =  summe  c[j]*exp(2*pi*i*k*j/n),  k= 0,...,n-1.
                       j=0

            Dabei muss n von der Form n = 2^p sein.

Parameter
Eingabe:    p        -  definiert n = 2^p
            analyse  -  steuert Analyse bzw. Synthese, d.h. a) oder b):
                        1:  a)
                        0:  b)
            u, v     -  Im Fall a) sind u und v Real- bzw. Imaginaerteil
                        des Vektors w,
                        im Fall b) sind u und v Real- bzw. Imaginaerteil
                        des Vektors c.

Ausgabe:    u, v     -  Im Fall a) sind u und v Real- bzw. Imaginaerteil
                        des Vektors c,
                        im Fall b) sind u und v Real- bzw. Imaginaerteil
                        des Vektors w.

Literatur:  J. Stoer, Einfuehrung in die Numerische Mathematik I
            Springer-Verlag, Berlin
            R. Sauer, I. Szabo, Mathematische Hilfsmittel des
            Ingenieurs, Teil III, Springer-Verlag, Berlin

--------------------------------------------------------------------- */

inline void NumericalLaplace::umkehr(int *m,
                   int n,
                   int i,
                   int *j,
                   int *k,
                   int nh,
                   int nq,
                   int ne)
{
    (*m)++;
    switch(*m)
    {
        case 1:
        case 3:
        case 5:
        case 7:
            *j = (*j)+nh;
            return;

        case 2:
        case 6:
            *j = (*j)-nq;
            return;

        case 4:
            *j = (*j)-ne;
            return;

        case 8:
            *j = 0;
            *k = 1;
            *m = i;
            break;
    }

    while(*m != 0)
    {
        *m = 2*(*m);
        if (*m >= n)
        {
            *m = (*m)-n;
            *j = (*j)+(*k);
        }
        *k = 2*(*k);
    }
    return;
}                        /* umkehr */

void NumericalLaplace::xfft(int p,
         int analyse,
         double *u,
         double *v)
{
    int n, i, imax, j=0, k, kl, kmax, l, m, nh, nq, ne;
    double ci, si, dc, ds, h, pr, pi, qr, qi, faktor;


    n = 1;
    for (i = 1; i <= p; i++)
        n *= 2;
    faktor = 1/(double)n;
    nh = n/2;
    nq = nh/2;
    ne = (int) Round((double)nq*2.5);
    l = n;
    imax = 0;
    h = -4;

    while(1)
    {
        if (l <= 1)
            break;
        l /= 2;
        m = 7;
        dc = h*0.5;
        ci = 1;
        si = 0;
        ds = sqrt(-dc*(2+dc));
        if (analyse) ds = -ds;

        for(i = 0; i <= imax; i++) {
            umkehr(&m, n, i, &j, &k, nh, nq, ne);
            kmax = j+l-1;
            for(k = j; k <= kmax; k++)
            {
                kl = k+l;
                pr = u[k];
                pi = v[k];
                qr = u[kl]*ci-v[kl]*si;
                qi = u[kl]*si+v[kl]*ci;
                u[k] = pr+qr;
                v[k] = pi+qi;
                u[kl] = pr-qr;
                v[kl] = pi-qi;
            }
            ci += dc;
            dc += h*ci;
            si += ds;
            ds += h*si;
        }

        imax = imax+imax+1;
        h = h/(2+sqrt(4+h));
    }

    imax = n-1;
    m = 7;
    for(i = 0; i <= imax; i++)
    {
        if (analyse)
        {
            u[i] *= faktor;
            v[i] *= faktor;
        }
        umkehr(&m, n, i, &j, &k, nh, nq, ne);
        if (j < i)
        {
            h = u[i];
            u[i] = u[j];
            u[j] = h;
            h = v[i];
            v[i] = v[j];
            v[j] = h;
        }
    }

}                           /* xfft */

double NumericalLaplace::Round(double x)
{
    if ((x - floor(x)) < 0.5) return(floor(x));
    else return(ceil(x));
}

/* --------------------------------------------------------------------

            DOKUMENTATION

Funktion:   linvweex

Zweck:      linvweex berechnet die Koeffizienten einer Entwicklung
            nach Laguerre-Funktionen der inversen Laplace-
            Transformierten f(t) einer Funktion F(s), die fuer
            komplexes s, Re(s) > c definiert ist:

                     /00
            F(s) =   |  exp(-s*t)f(t)dt, Re(s) > c
                    /0

            Es wird das Verfahren von Weeks benutzt, das auf einer
            konformen Abbildung und trigonometrischer Interpolation
            beruht.

            Die berechnete Laguerre-Entwicklung

                         n
            exp(c*t) * summe a  * phi (t/T),
                        i=0   i

            wobei   phi (t) = exp(-t/2) * L (t)
                       i                   i

            wird an der vorgegebenen Stelle t ausgewertet

Parameter
Eingabe:    F        - Pointer auf Funktion F
            p        - 2 hoch p = n+1 = Anzahl der Koeffizienten
            c        - Statt F(s) wird F(s + c) ausgewertet,
                       das Ergebnis muss dann mit exp(c*t)
                       multipliziert werden. Eine Wahl von c > 0
                       ist notwendig fuer Funktionen F, die
                       rechts von der imaginaeren Achse oder auf
                       der imaginaeren Achse Singularitaeten
                       besitzen.
            grosst   - Skalierungsparameter T zur Verbesserung
                       der Konvergenz
            rho		 - Parameter, der bestimmt, wo die Funktion
                       f ausgewertet wird, entweder rho = 1 oder
                       rho in der Naehe von 1.
            m        - Anzahl der Auswertungspunkte
            t        - Array (1 ... m) mit Auswertungspunkten

Ausgabe:    y        - Array (1 ... m) mit berechneten Werten
            a        - berechneter Vektor der Koeffizienten
            wert     - Wert der berechneten Laguerre-Entwicklung an
                       der Stelle t

Resultat:	Fehlerindikator
                     - 0 : o.k.
                     - 1 : falsche Eingabe (n, m, rho, grosst)

Literatur:  W.T. Weeks, Numerical Inversion of Laplace Transforms
            Using Laguerre Functions, JACM 13(1966), 419-426
--------------------------------------------------------------------- */

int NumericalLaplace::linvweex(   mathFunctionEvaluator *EvalA,
                                        int p,
                                        double c,
                                        double grosst,
                                        double rho,
                                        int m,
                                        double *t,
                                        double *a)
{
    double delta = 0.0000001;
    double pidurchnp1;
    double zweipidurchnp1;
    double einsdurchzweit;
    int n, np1, i;
    double *beta;

    n = 1;
    for (i = 1; i <= p; i++)
        n *= 2;
    n -= 1;

    if (n <= 0 || rho <= 0 || grosst <= 0 || m <= 0)
        return 1;

    beta = davec(0, n);

    np1 = n+1;
    pidurchnp1 = M_PI/np1;
    zweipidurchnp1 = 2*M_PI/np1;
    einsdurchzweit = 1.0/(2.0*grosst);

    /* Interpolations-Punkte der Ordnung np1 = n+1 berechnen */
    if (rho == 1)
    {
        double si;
        complex <double> s, u, w, w1, w2;

        si = einsdurchzweit*cotan((delta)*pidurchnp1);
        //std::complex <long double> A(c + si);
        s = EvalA->eval(std::complex <long double>(c, si));
        u = complex<double> (einsdurchzweit, si);
        w1 = u*s;

        //s = F(c - I*si);
        u = complex<double> (einsdurchzweit, -si);
        w2 = u*s;

        a[0] = (w1.real() + w2.real())/2.0;
        beta[0] = (w1.imag() + w2.imag())/2.0;

        /* Funktionswerte berechnen */
        for (i = 1; i <= n/2 + 1; i++)
        {
            double si = einsdurchzweit*cotan((i)*pidurchnp1);

            s = EvalA->eval(complex<long double> (c, si));
            u = complex<double>(einsdurchzweit, si);
            w = u*s;

            a[i] = w.real();
            beta[i] = w.imag();
        }
        for (i = 0; i <= n/2; i++)
        {
            a[n-i] = a[i+1];
            beta[n-i] = -beta[i+1];
        }
    }
    else
    {
        double rhoq = rho*rho;
        double zweirho = 2*rho;

        /* Funktionswerte berechnen */
        for (i = 0; i <= n/2 + 1; i++)
        {
            double z, sri, sii;
            complex <double> u, s, w;

            z = 1 + rhoq - zweirho*cos(zweipidurchnp1*i);
            sri = einsdurchzweit*(1-rhoq)/z;
            sii = einsdurchzweit*zweirho*sin(zweipidurchnp1*i);

            s = EvalA->eval(complex<long double>(c + sri, sii));
            u = complex<double> (einsdurchzweit + sri, sii);
            w = u*s;

            a[i] = w.real();
            beta[i] = w.imag();
        }

        for (i = 0; i <= n/2; i++)
        {
            a[n-i] = a[i+1];
            beta[n-i] = -beta[i+1];
        }
    }

    /* Koeffizienten a[k] mit FFT berechnen */
    xfft(p, 1, a, beta);

    if (rho != 1)
    {
        double z = 1;

        for (i = 1; i <= n; i++)
        {
            z *= rho;
            a[i] = a[i]/z;
        }
    }

    dfvec(beta, 0, n);

    ThreadOutput.clear();
    /* Laguerre-Entwicklung in t auswerten */
    for (i = 1; i <= m; i++)
        ThreadOutput.append(QPointF(t[i], auinvlap(n, a, c, t[i], grosst)));

    return 0;
}                            /* linvweex */
/* --------------------------------------------------------------------

            DOKUMENTATION

Funktion:   auinvlap

Zweck:      auinvlap berechnet den Wert einer Entwicklung
            nach Laguerre-Funktionen an der Stelle t >= 0

              n
            summe a  * phi (t),
             i=0   i      i

            wobei   phi (t) = exp(-t/2) * L (t)
                       i                   i

            wie er zur Auswertung der mit invlapr berechneten
            Entwicklung gebraucht wird.

Parameter
Eingabe:    n        - Anzahl der Koeffizienten minus 1
            a        - Vektor der Koeffizienten
            c        - das Ergebnis wird mit exp(c*t)
                       multipliziert, siehe invlapr
            t        - Stelle fuer die Auswertung
            grosst   - Skalierungsfaktor T

Literatur:  R.A. Spinelli, Numerical Inversion of Laplace
            Transforms, SIAM J. Num. Anal. 3(1966), 636-649
--------------------------------------------------------------------- */

double NumericalLaplace::auinvlap(int n,
                double *a,
                double c,
                double t,
                double grosst)
{
    int i;
    double s;
    double t1 = t;
    double li, lim1, lim2;

    t = t/grosst;
    lim2 = 1.0;
    lim1 = 1.0 - t;
    s = a[0] + a[1]*lim1;

    for (i = 2; i <= n; i++)
    {
        li = ((2*i-1-t)*lim1-(i-1)*lim2)/i;
        s += a[i]*li;
        lim2 = lim1;
        lim1 = li;
    }

    if (t > 0)
        return exp(c*t1 - 0.5*t)*s;
    else
        return s;
}

double NumericalLaplace::cotan(double i)
{
    return(1 / tan(i));
}

double *NumericalLaplace::davec(long nl, long nh)
{
    double *v;

    v= (double *) malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
    if (!v)
        aerror("allocation failure in davec()");
    return v-nl+NR_END;
}


void NumericalLaplace::dfvec(double *v, long nl, long nh)
{
    free((FREE_ARG) (v+nl-NR_END));
}

void NumericalLaplace::aerror(char error_text[])
{
    fprintf(stderr,"NUMLIB Runtime error...\n");
    fprintf(stderr,"%s\n", error_text);
}
