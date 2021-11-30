#include <assert.h>
#include "Calculator.h"
#include "FileOperations.h"

#define IS_CALCULATOR_ERROR()                       \
    do                                              \
    {                                               \
        if (*calculatorError != CALCULATOR_NO_ERROR) \
        {                                           \
            return nullptr;                         \
        }                                           \
    } while(0)

#define ASSERT_OK()                         \
    do                                      \
    {                                       \
        assert(s               != nullptr); \
        assert(val             != nullptr); \
        assert(calculatorError != nullptr); \
    } while(0)

static const char* GetG(const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetE(const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetT(const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetP(const char *s, double *val, CalculatorErrorCode *calculatorError);
static const char* GetN(const char *s, double *val, CalculatorErrorCode *calculatorError);

static const char* Require(const char *s,const char symbol, CalculatorErrorCode *calculatorError)
{
    assert(s               != nullptr);
    assert(calculatorError != nullptr);

    if (*s == symbol) s++;
    else *calculatorError = CALCULATOR_SYNTAX_ERROR;

    return s;
}

static const char* GetG(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    s = GetE(s, val, calculatorError);
    IS_CALCULATOR_ERROR();

    s = Require(s, '$', calculatorError);

    return s;
}

static const char* GetE(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    s = GetT(s, val, calculatorError);
    IS_CALCULATOR_ERROR();

    while (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        double val2 = 0;
        s = GetT(s, &val2, calculatorError);
        IS_CALCULATOR_ERROR();
        if (op == '+') *val += val2;
        else           *val -= val2;
    }

    return s;
}

static const char* GetT(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    s = GetP(s, val, calculatorError);
    IS_CALCULATOR_ERROR();

    while (*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        double val2 = 0;
        s = GetP(s, &val2, calculatorError);
        IS_CALCULATOR_ERROR();
        if (op == '*') *val *= val2;
        else           *val /= val2;
    }

    return s;
}

static const char* GetP(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    if (*s == '(')
    {
        s++;
        *val = 0;
        s = GetE(s, val, calculatorError);
        IS_CALCULATOR_ERROR();
        s = Require(s, ')', calculatorError);
        return s;
    }
    else
    {
        s = GetN(s, val, calculatorError);
        IS_CALCULATOR_ERROR();
        return s;
    }
}

static const char* GetN(const char *s, double *val, CalculatorErrorCode *calculatorError)
{
    ASSERT_OK();

    *val = 0;
    const char *oldS = s;
    while ('0' <= *s && *s <= '9')
    {
        *val += *val * 10 + (*s - '0');
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
    s = GetG(s, &val, calculatorError);

    free(str);
    fclose(finput);
    return val;
}
