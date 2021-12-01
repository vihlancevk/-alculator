#include <assert.h>
#include <math.h>
#include <string.h>
#include "Calculator.h"
#include "FileOperations.h"

#define IS_CALCULATOR_ERROR()                        \
    do                                               \
    {                                                \
        if (*calculatorError != CALCULATOR_NO_ERROR) \
        {                                            \
            return nullptr;                          \
        }                                            \
    } while(0)

#define ASSERT_OK()                         \
    do                                      \
    {                                       \
        assert(s               != nullptr); \
        assert(val             != nullptr); \
        assert(calculatorError != nullptr); \
    } while(0)

static const char* GetBegin      (const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetAddOrSub   (const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetMulOrDiv   (const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetDegree     (const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetSinOrCos   (const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetParenthesis(const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetNumber     (const char *s, double *val, CalculatorErrorCode *calculatorError);

static const char* Require(const char *s,const char symbol, CalculatorErrorCode *calculatorError)
{
    assert(s               != nullptr);
    assert(calculatorError != nullptr);

    if (*s == symbol) s++;
    else *calculatorError = CALCULATOR_SYNTAX_ERROR;

    return s;
}

static const char* GetBegin(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    s = GetAddOrSub(s, val, calculatorError);
    IS_CALCULATOR_ERROR();

    s = Require(s, '$', calculatorError);

    return s;
}

static const char* GetAddOrSub(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    s = GetMulOrDiv(s, val, calculatorError);
    IS_CALCULATOR_ERROR();

    while (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        double val2 = 0;
        s = GetMulOrDiv(s, &val2, calculatorError);
        IS_CALCULATOR_ERROR();
        if (op == '+') *val += val2;
        else           *val -= val2;
    }

    return s;
}

static const char* GetMulOrDiv(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    s = GetDegree(s, val, calculatorError);
    IS_CALCULATOR_ERROR();

    while (*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        double val2 = 0;
        s = GetDegree(s, &val2, calculatorError);
        IS_CALCULATOR_ERROR();
        if (op == '*') *val *= val2;
        else           *val /= val2;
    }

    return s;
}

static const char* GetDegree(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    s = GetSinOrCos(s, val, calculatorError);

    while (*s == '^')
    {
        double val2 = 0;
        s++;
        s = GetSinOrCos(s, &val2, calculatorError);
        IS_CALCULATOR_ERROR();
        *val = pow(*val, val2);
    }

    return s;
}

static const char* GetSinOrCos(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();
    if (strncmp("sin", s, 3) == 0 || strncmp("cos", s, 3) == 0)
    {
        char op[3] = {};
        strncpy(op, s, 3);
        double val2 = 0;
        s = s + 3;
        s = GetParenthesis(s, &val2, calculatorError);
        if (strcmp("sin", op) == 0) { *val = sin(val2); }
        else                        { *val = cos(val2); }
    }
    else
    {
        s = GetParenthesis(s, val, calculatorError);
    }

    return s;
}

static const char* GetParenthesis(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    if (*s == '(')
    {
        s++;
        *val = 0;
        s = GetAddOrSub(s, val, calculatorError);
        IS_CALCULATOR_ERROR();
        s = Require(s, ')', calculatorError);
        return s;
    }
    else
    {
        s = GetNumber(s, val, calculatorError);
        IS_CALCULATOR_ERROR();
        return s;
    }
}

static const char* GetNumber(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    *val = 0;
    const char *oldS = s;
    while ('0' <= *s && *s <= '9')
    {
        *val = *val * 10 + (*s - '0');
        s++;
    }
    if (oldS == s) *calculatorError = CALCULATOR_SYNTAX_ERROR;
    return s;
}

#undef IS_CALCULATOR_ERROR
#undef ASSERT_OK

double CalculatExpression(const char *nameFinput, CalculatorErrorCode *calculatorError)
{
    assert(nameFinput      != nullptr);
    assert(calculatorError != nullptr);

    FILE *finput = fopen(nameFinput, "r");

    int numberBytesSize = GetFileSize(finput);
    char *str = (char*)calloc(numberBytesSize + 1, sizeof(char));
    str = (char*)ReadFile(finput, str, numberBytesSize);

    const char *s = str;
    double val = 0;
    s = GetBegin(s, &val, calculatorError);

    free(str);
    fclose(finput);
    return val;
}
