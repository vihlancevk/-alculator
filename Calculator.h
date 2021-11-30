#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <stdio.h>
#include <stdlib.h>

enum CalculatorErrorCode
{
    CALCULATOR_NO_ERROR,
    CALCULATOR_SYNTAX_ERROR
};

double CalculatExpression(const char *nameFinput, CalculatorErrorCode *calculatorError);

#endif // CALCULATOR_H_
