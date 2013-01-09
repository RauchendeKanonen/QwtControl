#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define pi 3.1416
#define e 2.7183



double *convolve(const double *Signal, size_t SignalLen,
                 const double *Kernel, size_t KernelLen)
{
    double *Result = (double*)malloc(sizeof(double)*(SignalLen + KernelLen));
    size_t n;

    for (n = 0; n < SignalLen + KernelLen - 1; n++)
    {
        size_t kmin, kmax, k;

        Result[n] = 0;

        kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
        kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

        for (k = kmin; k <= kmax; k++)
        {
            Result[n] += Signal[k] * Kernel[n - k];
        }

    }
    return Result;
}
double *removeTimeConstant(double *y, int len, double TimeConst, double SampelInterval)
{
    double *x = (double*) malloc(len*sizeof(double));
    double a = TimeConst, T = SampelInterval;

    double sum = 0;

    for(int i = 1 ; i < len ; i ++)
    {
        x[i] = (y[i]-pow(e, -T/a)*y[i-1])*1/(T/a);
    }
    return x;
}

#define STEPRESPLEN 10000
#define IMPRESPLEN  10000
int main(int argc, char *argv[])
{
    double IResponseSensor[IMPRESPLEN];
    double StepResponseX[STEPRESPLEN];
    double IResSum = 0;
    double T = 0.01, tau = 10, tausen = 10;

    double inval = 0;
    for(int i = 0 ; i < STEPRESPLEN ; i ++, inval += T)
    {
        StepResponseX[i] = 1-pow(e, -inval/tau);
        //StepResponseX[i] = sin(inval/tau)+(double)rand()/((double)RAND_MAX*10.0);
        //StepResponseX[i] = 1-pow(e, -inval/tau)+(double)rand()/((double)RAND_MAX*10.0);
    }

    inval = 0;
    for(int i = 0 ; i < IMPRESPLEN ; i ++, inval += T)
    {
        IResponseSensor[i] = pow(e, -inval/tausen);
        IResSum += IResponseSensor[i];
    }

    double *pSig = convolve( IResponseSensor, IMPRESPLEN, StepResponseX, STEPRESPLEN);


    for(int i = 0 ; i < IMPRESPLEN+STEPRESPLEN ; i ++)
    {
        pSig[i]/=IResSum;
        printf("%f\n", pSig[i]);
    }

    printf("\n");


    for(int i = 0 ; i < STEPRESPLEN ; i ++)
        printf("%f\n", StepResponseX[i]);

    printf("\n");

    double *OrigSig = removeTimeConstant(pSig, IMPRESPLEN+STEPRESPLEN, 10 ,T);

    for(int i = 0 ; i < IMPRESPLEN+STEPRESPLEN ; i ++)
        printf("%f\n", OrigSig[i]);

}




