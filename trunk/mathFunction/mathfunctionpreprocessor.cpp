#include "mathfunctionpreprocessor.h"



mathFunctionpreprocessor::mathFunctionpreprocessor(QString ExpressionA)
{
    ExpressionString =  ExpressionA;
    preformatExponents(&ExpressionString);
    state = preprocessExponents(&ExpressionString);
}

bool mathFunctionpreprocessor::getState(void)
{
    return state;
}

bool mathFunctionpreprocessor::preformatExponents(QString *Expression)
{
    if(Expression->contains("^"))
    {
        for(int index = 0 ; index >=0 ; )
        {
            index = Expression->indexOf("^", index+1);
            //no more potences
            if(index == -1)
                break;

            //isolate potence with brackets
            if(Expression->at(index+1) != '(')
            {
                bool Symbol = (Expression->at(index+1) == '-' || Expression->at(index+1) == '+');
                bool alpha = Expression->at(index+1).isLetter();
                if(Symbol)
                    alpha = Expression->at(index+2).isLetter();
                for( int i = index+1 ; i < Expression->count() ; i ++ )
                {
                    if(alpha && Expression->at(i+1).isLetter())
                    {

                    }
                    else if(alpha)
                    {
                        Expression->insert(i+1, ')');
                        Expression->insert(index+1, '(');
                        break;
                    }
                    if(!alpha && ((Expression->at(i+1).isDigit()) || (Expression->at(i+1) == '.')))
                    {

                    }
                    else if(!alpha)
                    {

                        Expression->insert(i+1, ')');
                        Expression->insert(index+1, '(');
                        break;
                    }
                }
            }

            //isolate base with brackets
            if(Expression->at(index-1) != ')')
            {
                bool alpha = Expression->at(index-1).isLetter();
                for( int i = index ; i > 0 ; i-- )
                {
                    if(alpha && Expression->at(i-1).isLetter())
                    {

                    }
                    else if(alpha)
                    {
                        Expression->insert(index, ')');
                        Expression->insert(i, '(');
                        break;
                    }
                    if(!alpha && ((Expression->at(i-1).isDigit()) || (Expression->at(i-1) == '.')))
                    {

                    }
                    else if(!alpha)
                    {
                        Expression->insert(index, ')');
                        Expression->insert(i, '(');
                        break;
                    }
                }
                index += 2;
            }

        }
    }
    return true;
}

bool mathFunctionpreprocessor::preprocessExponents(QString *Expression)
{
    if(Expression->contains("^"))
    {
        int index;
        for(index = 0 ; index >=0 ; )
        {
            index = Expression->indexOf("^", index+1);

            int ExpBegin = -1;
            int ExpEnd = -1;

            //no more potences
            if(index == -1)
                break;
            //determine the base
            if(Expression->at(index-1) == ')')
            {
                int closing_brackets = 1;
                int i;
                for( i = -1 ; closing_brackets > 0 && 0 <=(index-1+i); i --)
                {
                    if(Expression->at(index-1+i) == '(')
                        closing_brackets--;
                    if(Expression->at(index-1+i) == ')')
                        closing_brackets++;
                }
                if(closing_brackets == 0)
                {
                    QString Base = Expression->left(index);
                    Base = Base.right(-i);
                    ExpBegin = index+i;
                }
            }
            //determine exponent
            if(Expression->at(index+1) == '(')
            {
                int opening_brackets = 1;
                int i;
                for( i = 1 ; opening_brackets > 0 && Expression->count() > (index+1+i); i ++)
                {
                    if(Expression->at(index+1+i) == '(')
                        opening_brackets++;
                    if(Expression->at(index+1+i) == ')')
                        opening_brackets--;
                }
                if(opening_brackets == 0)
                {
                    QString Exponent = Expression->right(Expression->length()-index);
                    Exponent = Exponent.right(i);
                    ExpEnd = index+i;
                }

            }
            if(ExpEnd > 0 && ExpBegin > 0)
            {
                Expression->replace(index, 1,",");

                Expression->insert(ExpEnd, ")");
                Expression->insert(ExpBegin, "pow(");
            }
            else
            {
                Message = "Missing Bracket";
                return false;
            }
        }
    }
    return true;
}


QString mathFunctionpreprocessor::preprocessedString(void)
{
    if(state)
        return ExpressionString;


}
