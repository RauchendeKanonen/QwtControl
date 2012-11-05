#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define pi 3.141
#define e 2.781




double *convolve(double *Kernel, int KernelLenght, double *Signal, int SignalLength)
{
    double *OutSig = (double*)malloc(sizeof(double)*(SignalLength+KernelLenght));

    for(int f = 0 ; f < (SignalLength+KernelLenght) ; f ++)
        OutSig[f] = 0;

    for(int i = 0 ; i < SignalLength ; i ++)
        for(int y = 0; y < KernelLenght ; y ++)
            OutSig[i+y] += Signal[i] * Kernel[y];


    return OutSig;
}


int main(int argc, char *argv[])
{
    double IResponseSensor[100];
    double StepResponseX[100];

    double a = 10, T = 0.01, tau = 0.1, tausen = 0.1;


    for(int i = 1 ; i < 100 ; i ++)
    {
        StepResponseX[i] = 1-pow(e, -i*T*1/tau);
    }

    for(int i = 1 ; i < 100 ; i ++)
    {
        IResponseSensor[i] = pow(e, -i*T*1/tausen);
    }

    double *pSig = convolve(StepResponseX, 100, IResponseSensor, 100);

    for(int i = 1 ; i < 100 ; i ++)
    {

        if(IResponseSensor[i-1]<0.632 && IResponseSensor[i]>0.632)
            printf("\n");
        printf("%f\n", IResponseSensor[i]);
    }
}


/*
int main(int argc, char *argv[])
{
    double y[100], x[100];
    double a = 10, T = 0.01, tau = 0.1;

    y[0] = 0;
    for(int i = 1 ; i < 100 ; i ++)
    {

        y[i] = 1-pow(e, -i*T*1/tau);
        x[i] = y[i]-pow(e, -a*T)*y[i-1];

        if(y[i-1]<0.632 && y[i]>0.632)
            printf("\n");
        printf("%f\n", y[i]);
    }
}*/
