#include "../include/Calculator.h"
#include "../include/FileOperations.h"

const char *NAME_INPUT_FILE = "../res/data.txt";

int main()
{
    Parser parser = {};

    FILE *finput = fopen(NAME_INPUT_FILE, "r");

    int numberBytesSize = GetFileSize(finput);
    parser.str = (char*)calloc(numberBytesSize + 1, sizeof(char));
    parser.str = (char*)ReadFile(finput, parser.str, numberBytesSize);

    parser.calculatorError = CALCULATOR_NO_ERROR;

    double val = CalculateExpression(&parser);
    if (parser.calculatorError != CALCULATOR_NO_ERROR)
    {
        printf("%g\n", val);
    }
    else
    {
        printf("%g\n", val);
    }

    free(parser.str);
    fclose(finput);
    return 0;
}
