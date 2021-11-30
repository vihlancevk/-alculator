#include "Calculator.h"

const char *nameFinput = "data.txt";

int main()
{
    CalculatorErrorCode calculatorError = CALCULATOR_NO_ERROR;
    double val = CalculatExpression(nameFinput, &calculatorError);
    if (calculatorError != CALCULATOR_NO_ERROR)
    {
        printf("CALCULATOR_SYNTAX_ERROR!\n");
    }
    else
    {
        printf("%g\n", val);
    }

    return 0;
}
