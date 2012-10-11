#include <iostream>
#include "mathfunctioncompiler.h"
#include "mathfunctionevaluator.h"
#include "mathfunctionpreprocessor.h"
#include "mpParser.h"

using namespace mup;
using namespace std;

long timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long msec;
  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
  return msec;
}

int main(int argc, char *argv[])
{
    ParserX Parser;
    Parser.SetExpr(argv[1]);
    Value Val = 1.1;
    Variable Var(&Val);
    Parser.DefineVar("k", &Var);

    mathFunctionEvaluator *Evaluator = new mathFunctionEvaluator(Parser, QString("s"), QString("Function1"));

    struct timeval starttime;
    struct timeval endtime;
    for(;;)
    {
        gettimeofday( &starttime, NULL);
        for(int i = 0 ; i < 10e7 ; i++)
        {
            Evaluator->eval(1.0);
        }
        gettimeofday( &endtime, NULL);
        printf("10e7 executions took %dms\n", timevaldiff(&starttime, &endtime));
    }
}

