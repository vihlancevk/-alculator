#include <assert.h>
#include <math.h>
#include <string.h>
#include "../include/Calculator.h"

#define IS_CALCULATOR_ERROR()                               \
    do                                                      \
    {                                                       \
        if (parser->calculatorError != CALCULATOR_NO_ERROR) \
        {                                                   \
            return 0;                                       \
        }                                                   \
    } while(0)

static double GetG    (Parser *parser);
static double GetE    (Parser *parser);
static double GetT    (Parser *parser);
static double GetPower(Parser *parser);
static double GetUnary(Parser *parser);
static double GetP    (Parser *parser);
static double GetN    (Parser *parser);

const int NUMBER_UNARY_OPERATIONS = 2;

UnaryOperation unaryOperation[NUMBER_UNARY_OPERATIONS] = {{ "sin", 3, sin},
                                                          { "cos", 3, cos}};

static void Require(Parser *parser, const char symbol)
{
    assert(parser != nullptr);

    if (*(parser->str + parser->curOffset) == symbol) parser->curOffset++;
    else parser->calculatorError = CALCULATOR_SYNTAX_ERROR;
}

static double GetG(Parser *parser)
{
    assert(parser != nullptr);

    double val = GetE(parser);
    IS_CALCULATOR_ERROR();

    Require(parser, '$');

    return val;
}

static double GetE(Parser *parser)
{
    assert(parser != nullptr);

    double val = GetT(parser);
    IS_CALCULATOR_ERROR();

    while (*(parser->str + parser->curOffset) == '+' || *(parser->str + parser->curOffset) == '-')
    {
        char op = *(parser->str + parser->curOffset);
        parser->curOffset++;
        double val2 = GetT(parser);
        IS_CALCULATOR_ERROR();
        if (op == '+') val += val2;
        else           val -= val2;
    }

    return val;
}

static double GetT(Parser *parser)
{
    assert(parser != nullptr);

    double val = GetPower(parser);
    IS_CALCULATOR_ERROR();

    while (*(parser->str + parser->curOffset) == '*' || *(parser->str + parser->curOffset) == '/')
    {
        char op = *(parser->str + parser->curOffset);
        parser->curOffset++;
        double val2 = GetPower(parser);
        IS_CALCULATOR_ERROR();
        if (op == '*') val *= val2;
        else           val /= val2;
    }

    return val;
}

static double GetPower(Parser *parser)
{
    assert(parser != nullptr);

    double val = GetP(parser);

    while (*(parser->str + parser->curOffset) == '^')
    {
        parser->curOffset++;
        double val2 = GetP(parser);
        IS_CALCULATOR_ERROR();
        val = pow(val, val2);
    }
}

static double GetP(Parser *parser)
{
    assert(parser != nullptr);

    double val = 0;

    if (*(parser->str + parser->curOffset) == '(')
    {
        parser->curOffset++;
        val = GetE(parser);
        IS_CALCULATOR_ERROR();
        Require(parser, ')');
    }
    else
    {
        size_t curOffset = parser->curOffset;
        val = GetUnary(parser);
        if (curOffset == parser->curOffset) { val = GetN(parser); }
        IS_CALCULATOR_ERROR();
    }

    return val;
}

static double GetUnary(Parser *parser)
{
    assert(parser != nullptr);

    double val = 0;

    for (int i = 0; i < NUMBER_UNARY_OPERATIONS; i++)
    {
        if (strncmp(unaryOperation[i].str, (parser->str + parser->curOffset), unaryOperation[i].strSize) == 0)
        {
            char op[unaryOperation[i].strSize] = {};
            strncpy(op, parser->str + parser->curOffset, unaryOperation[i].strSize);
            parser->curOffset += unaryOperation[i].strSize;
            double val2 = GetP(parser);
            if (strcmp(unaryOperation[i].str, op) == 0) { val = unaryOperation[i].operation(val2); }
            else                                        { val = unaryOperation[i].operation(val2); }
        }
    }

    return val;
}

static double GetN(Parser *parser)
{
    assert(parser != nullptr);

    double val = 0;
    const char *oldS = parser->str;
    while (*(parser->str + parser->curOffset) >= '0' && '9' >= *(parser->str + parser->curOffset))
    {
        val = val * 10 + (*(parser->str + parser->curOffset) - '0');
        parser->curOffset++;
    }
    if (oldS == (parser->str + parser->curOffset)) parser->calculatorError = CALCULATOR_SYNTAX_ERROR;

    return val;
}

#undef IS_CALCULATOR_ERROR

double CalculateExpression(Parser *parser)
{
    assert(parser != nullptr);

    double val = GetG(parser);

    return val;
}
