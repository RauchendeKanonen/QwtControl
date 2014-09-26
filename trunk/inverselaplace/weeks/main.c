#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#define NR_END 1
#define FREE_ARG char*

// http://www.cs.hs-rm.de/~weber/lapinv/weeks/weeks.htm

double acosh(double x);
double U(double x);
double *davec(long nl, long nh);
void dfvec(double *v, long nl, long nh);
double cotan(double i);
static void aerror(char error_text[]);
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

int linvweex(complex double (*F)(complex double s), 
		     int p,
		     double c,
			 double grosst,
			 double rho,
		     int m,
		     double *t,
			 double *y,
		     double *a)
{
    double auinvlap(int n, double *a, double c, double t, double grosst);
	void xfft(int p, int analyse, double *u, double *v);
	double delta = 0.0000001;
	double pidurchnp1;
	double zweipidurchnp1;
	double einsdurchzweit;
	int n, np1, i;
	double *beta;	

	n = 1; 
	for (i = 1; i <= p; i++) n *= 2;
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
		complex double s, u, w, w1, w2;

		si = einsdurchzweit*cotan((delta)*pidurchnp1); 
		s = F(c + I*si);
		u = einsdurchzweit + I*si;
		w1 = u*s;

		s = F(c - I*si);
		u = einsdurchzweit - I*si;
		w2 = u*s;

		a[0] = (creal(w1) + creal(w2))/2.0;
		beta[0] = (cimag(w1) + cimag(w2))/2.0;

		/* Funktionswerte berechnen */
		for (i = 1; i <= n/2 + 1; i++) 
		{
			double si = einsdurchzweit*cotan((i)*pidurchnp1); 

			s = F(c + I*si);
			u = einsdurchzweit + I*si;
			w = u*s;
			
			a[i] = creal(w);
			beta[i] = cimag(w);			
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
			complex double u, s, w;

			z = 1 + rhoq - zweirho*cos(zweipidurchnp1*i);
			sri = einsdurchzweit*(1-rhoq)/z;
			sii = einsdurchzweit*zweirho*sin(zweipidurchnp1*i); 
			
			s = F(c + sri + I*sii);
			u = einsdurchzweit + sri + I*sii;
			w = u*s;
			
			a[i] = creal(w);
			beta[i] = cimag(w);			
		}

		for (i = 0; i <= n/2; i++) 
		{
			a[n-i] = a[i+1];	
			beta[n-i] = -beta[i+1];	
		}
	}

   	/* Koeffizienten a[k] mit FFT berechnen */
	xfft(p, 1, a, beta);

	if (rho != 1) {
		double z = 1;

		for (i = 1; i <= n; i++) {
			z *= rho;
			a[i] = a[i]/z;
		}
	}
		
	dfvec(beta, 0, n);

	/* Laguerre-Entwicklung in t auswerten */
	for (i = 1; i <= m; i++) 
		y[i] = auinvlap(n, a, c, t[i], grosst);

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

double auinvlap(int n, 
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






double cotan(double i) 
{ 
	return(1 / tan(i)); 
}			 

double *davec(long nl, long nh)
{
	double *v;

	v= (double *) malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) aerror("allocation failure in davec()");
	return v-nl+NR_END;
}

void dfvec(double *v, long nl, long nh)
{
	free((FREE_ARG) (v+nl-NR_END));
}

static void aerror(char error_text[])
{
	fprintf(stderr,"NUMLIB Runtime error...\n");
	fprintf(stderr,"%s\n", error_text);
	exit(1);
}

#define K 1.0
#define M 20.0
#define B 0.5
#define dt 0.0


//validation
#define T 20.0
#define a 0.4

static complex double F(complex double s)
{
	complex double result;
	
	//result = cexp(-5.0*s)/(K+s*B+M*s*s)*1/s;
	
	//result = 1/(cexp(dt*s)*K+cexp(dt*s)*s*B+cexp(dt*s)*M*s*s+1)*1/s;
	
	result = cexp(-T*s)/(1+s/a)*1/s;

	return result;	
}

void putVector(double *x, double *y, int len)
{
	int i;
	for( i = 0 ; i < len ; i ++)
		printf("%f %f\n", x[i], y[i]);
}

int main(int argc, char *argv[])
{
	int m = 10000;
	int p = 12;
	int n = pow(2.0, p)-1;
	
	double t[m+1];
	double y[m+1];
	double coeff[n];
	
	int i;
		
	for( i = 0 ; i <= m ; i ++ )
	{
		y[i] = 0.0;
		t[i] = i/(double)m*100;
	}
	
	
	
	
	
	
	/* (Fptr, NCoeff, convergence abzissica, convergence parm, auswetungsposition, num of points*/
	int ret  = linvweex(F, p, 0.1, 5/*0.1*/, 1.0, m, t, y, coeff);
	
	putVector(t, y, m);
}
