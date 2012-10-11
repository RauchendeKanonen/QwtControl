#include<math.h>

double f(double s)
{
    return 1/(1+1/35.911600*s)*1/s   ;
}
char *indepVarName(void)
{
  return "s";
}
