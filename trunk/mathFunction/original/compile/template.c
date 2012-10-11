#include<math.h>

double f(double s)
{
    return s+1.100000/(s+1)*1/s+1/(s*s*s+1) ;
}
char *indepVarName(void)
{
  return "s";
}
