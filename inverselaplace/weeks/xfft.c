#include <math.h>

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

static void umkehr(int *m, 
				   int n, 
				   int i, 
				   int *j, 
				   int *k, 
				   int nh, 
				   int nq, 
				   int ne)
{
	(*m)++;
	switch(*m) {
	case 1:
	case 3:
	case 5:
	case 7: 
		*j = (*j)+nh;
		goto L999;

	case 2:
	case 6: 
		*j = (*j)-nq;
		goto L999;

	case 4: 
		*j = (*j)-ne;
		goto L999;

	case 8: 
		*j = 0;
		*k = 1;
		*m = i;
		break;
	}

	while(*m != 0) {
		*m = 2*(*m);
		if (*m >= n) {
			*m = (*m)-n;
			*j = (*j)+(*k);
		}
		*k = 2*(*k);
	}
L999: 
	return;
}                        /* umkehr */

void xfft(int p, 
		 int analyse, 
		 double *u, 
		 double *v)
{
	int n, i, imax, j, k, kl, kmax, l, m, nh, nq, ne;
	double ci, si, dc, ds, h, pr, pi, qr, qi, faktor;
	double Round();

	n = 1;
	for (i = 1; i <= p; i++) n *= 2;
	faktor = 1/(double)n;
	nh = n/2;
	nq = nh/2;
	ne = (int) Round((double)nq*2.5);
	l = n;
	imax = 0;
	h = -4;

L97:
	if (l <= 1) goto L98;
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
		for(k = j; k <= kmax; k++) {
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
	goto L97;

L98:
	imax = n-1;
	m = 7;
	for(i = 0; i <= imax; i++) {
		if (analyse) {
			u[i] *= faktor;
			v[i] *= faktor;
		}
		umkehr(&m, n, i, &j, &k, nh, nq, ne);
		if (j < i) {
			h = u[i];
			u[i] = u[j];
			u[j] = h;
			h = v[i];
			v[i] = v[j];
			v[j] = h;
		}
	}

}                           /* xfft */

double Round(double x)
{
	if ((x - floor(x)) < 0.5) return(floor(x));
	else return(ceil(x));
}