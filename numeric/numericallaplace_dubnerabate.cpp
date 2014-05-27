
/* linvdubf.c					  */
/* Testprogramm fuer linvdubf()   */
/* (C) 1992/2011 H. Weber         */
/* ANSI C                         */
/* Komplexe Version				  */
#include <numericallaplace.h>

#include <math.h>
#include <complex>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <time.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif
static double pi = M_PI;

using namespace std;

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



double NumericalLaplace::sqwave(double t)
{
    int r, tf;
    double tfl = floor(t);
    double eps = 1.0e-7;
    tf = (int) tfl;
    r = tf % 2;

    if (fabs(t - tfl) <= eps)
        return 0.5;
    else if (r == 0)
        return 0;
    else
        return 1;
}


/* --------------------------------------------------------------------

            DOKUMENTATION

Funktion:   linvdubf

Zweck:      linvdubf berechnet die Inverse f der Laplace-Transformierten

                      /00
            F(s) =   | exp(-s*t)f(t)dt, Re(s) > c0
                    0/

            mit dem Algorithmus von Dubner&Abate mit FFT

Parameter
Eingabe:    F		  - Pointer auf Funktion, die die Transformierte
                        F(s) berechnet, fuer komplexes Argument s
            p		  - 2^p = n = minimale Dimension der FFT
            ausw      - Zahl der beruecksichtigten Terme der unendlichen
                        Reihe
            a		  - Parameter a des Algorithmus (> c0)
            deltat    - Inkrement fuer Auswertungspunkte t[j]=j*deltat,
                        j = 1,...,anz
            anz       - Anzahle der auszugebenen Werte des Ergebnisses

Ausgabe     t         - eindim. Array (1 ... anz) mit den t-Werten, fuer
                        die f(t) zu berechnen wurde
            y		  -	eindim. Array (1 ... anz) mit den Resultaten
                        der Berechnungen fuer die Werte t[i]

Resultat	- Fehlerindikator
                      0 : ok.
                      1 : p < 1
                      2 : ausw < 1
                      3 : deltat <= 0
                      4 : anz < 1

Literatur:  H. Dubner, J. Abate, Numerical Inversion of Laplace
            Transforms and the Finite Fourier Transform, JACM 15(1968),
            115-123

Bemerkung:  Version mit komplexer Arithmetik (C99)
--------------------------------------------------------------------- */

int NumericalLaplace::linvdubf(mathFunctionEvaluator *EvalA,
             int p,
             int ausw,
             double a,
             double deltat,
             int anz,
             double *t,
             double *y)
{
    int i, j, k, n, m;
    double grosst;
    double *ar, *ai;
    double zpidurchn;
    void xfft(int p, int analyse, double *u, double *v);

    if (p < 1) return 1;
    if (ausw < 1) return 2;
    if (deltat <= 0) return 3;
    if (anz < 1) return 4;

    n = 1;
    for (i = 1; i <= p; i++) n *= 2;

    m = n/6;
    grosst = n/2.0;

#ifdef DEBUG
    printf("T = %lf p = %d n = %d m = %d tmax = %lf\n", grosst, p, n, m, m*deltat);
#endif

    while (m < anz) {
        n *= 2;
        p += 1;
        m = n/6;
#ifdef DEBUG
        printf("T = %lf p = %d n = %d m = %d tmax = %lf\n", grosst, p, n, m, m*deltat);
#endif
    }


    zpidurchn = 2*pi/n;

    ar = davec(0, n);
    ai = davec(0, n);

    /* compute input ar[k], ai[k] for FFT */
    for (k = 0; k <= n-1; k++)
    {
        int j;
        double sum = 0;

        for (j = -ausw; j <= ausw; j++)
        {
            complex <long double> z;
            complex <long double> arg(a, zpidurchn*(k + j*n));
            arg /= deltat;
            z = EvalA->eval(arg);
            z /= deltat;
            sum += z.real();
        }

        ar[k] = sum/n;
        ai[k] = 0;
    }

    /* FFT */
    this->xfft(p, 0, ar, ai);

    /* compute anz values of f at j*deltat */
    for (j = 1; j <= anz; j++)
    {
        t[j] = j*deltat;
        y[j] = ar[j]*2.0*exp(a*j);
    }


    ThreadOutput.clear();
    /* Laguerre-Entwicklung in t auswerten */
    for (i = 0; i < anz; i++)
        ThreadOutput.append(QPointF(t[i], y[i]));

    dfvec(ar, 0, n);
    dfvec(ai, 0, n);

    return 0;
}
