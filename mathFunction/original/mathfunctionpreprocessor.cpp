#include "mathfunctionpreprocessor.h"

QList < QPair < QString , double > > mathFunctionpreprocessor::getVariables(ParserX ExpressionX)
{
    QList  < QPair < QString , double > > VariablePairs;
    var_maptype vmap = ExpressionX.GetExprVar();
    for (var_maptype::iterator item = vmap.begin(); item!=vmap.end(); ++item)
    {
        QString VarName = QString::fromStdString(item->first);
        QString ValueString = QString::fromStdString((*(item->second)).ToString());

        double DblVal = strtod(ValueString.toStdString().c_str(), NULL);
        QPair< QString, double> Pair;
        Pair.first = VarName;
        Pair.second = DblVal;
        VariablePairs.append(Pair);
    }
    return VariablePairs;
}

mathFunctionpreprocessor::mathFunctionpreprocessor(ParserX ExpressionX, QString IndepVarName)
{
    ExpressionString = QString::fromStdString(ExpressionX.GetExpr());
    QList  < QPair < QString , double > > VariablePairs = getVariables(ExpressionX);

    for(int i = 0 ; i < VariablePairs.count() ; i ++ )
    {
        QString ValueString;
        ValueString.sprintf("%f", VariablePairs.at(i).second);
        if(IndepVarName != VariablePairs.at(i).first)
            ExpressionString.replace(VariablePairs.at(i).first, ValueString);
    }
}

QString mathFunctionpreprocessor::preprocessedString(void)
{
    return ExpressionString;
}
