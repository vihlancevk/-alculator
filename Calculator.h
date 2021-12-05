#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <stdio.h>
#include <stdlib.h>

enum CalculatorErrorCode
{
    CALCULATOR_NO_ERROR,
    CALCULATOR_SYNTAX_ERROR
};

struct Parser
{
    char* str;
    size_t curOffset;
    CalculatorErrorCode calculatorError;
};

struct UnaryOperation
{
    const char* str;
    size_t strSize;
    double (*operation) (double);
};

double CalculateExpression(Parser *parser);

#endif // CALCULATOR_H_
