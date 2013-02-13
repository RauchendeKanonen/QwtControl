#include"sorder.h"
double a;
double b;
char *VarNames[] = {"b", "a", NULL };
double  sorder(double s)
{
     return pow(2.71828182846,-b*s)*1/(pow(s,2.0)+1/a*s+1);
}
char**  getVar(void)
{
     return VarNames;
}
int  setVar(char *Varname, double Value)
{
     if(strcmp(Varname, "b") == 0)
            b = Value;
     if(strcmp(Varname, "a") == 0)
            a = Value;
}
