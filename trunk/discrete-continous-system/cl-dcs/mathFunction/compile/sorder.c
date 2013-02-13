#include"sorder.h"
double a;
char *VarNames[] = {"a", NULL };
double  sorder(double s)
{
     return 1/pow((pow(s,2.0)+1/a*s+1), 2);
}
char**  getVar(void)
{
     return VarNames;
}
int  setVar(char *Varname, double Value)
{
     if(strcmp(Varname, "a") == 0)
            a = Value;
}
