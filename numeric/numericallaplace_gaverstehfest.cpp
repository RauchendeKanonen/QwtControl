#include "numericallaplace.h"
#include <complex>


double NumericalLaplace::Factorial(int N)
{
    double x = 1;
    if (N > 1)
    {
        for (int i = 2; i <= N; i++)
            x = i * x;
    }
    return x;
}

void NumericalLaplace::InitStehfest(int N)
{
    ln2 = log(2.0);
    int N2 = N / 2;
    int NV = 2 * N2;
    V_Length = NV;
    V = new double[NV];
    int sign = 1;
    if ((N2 % 2) != 0)
        sign = -1;
    for (int i = 0; i < NV; i++)
    {
        int kmin = (i + 2) / 2;
        int kmax = i + 1;
        if (kmax > N2)
            kmax = N2;
        V[i] = 0;
        sign = -sign;
        for (int k = kmin; k <= kmax; k++)
        {
            V[i] = V[i] + (pow(k, N2) / Factorial(k)) * (Factorial(2 * k)
                / Factorial(2 * k - i - 1)) / Factorial(N2 - k) / Factorial(k - 1)
                / Factorial(i + 1 - k);
        }
        V[i] = sign * V[i];
    }
}
