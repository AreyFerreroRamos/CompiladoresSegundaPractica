#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;

int init_analisi_sintactic(char *filename)
{
    int error = EXIT_SUCCESS;

    yyout = fopen(filename, "w");

    if (yyout == NULL)
    {
        error = EXIT_FAILURE;
    }
    return error;
}

int analisi_semantic()
{
    int error;

    if (yyparse() == 0)
    {
        error = EXIT_SUCCESS;
    }
    else
    {
        error = EXIT_FAILURE;
    }
    return error;
}

int end_analisi_sintactic()
{
    int error;

    error = fclose(yyout);

    if (error == 0)
    {
        error = EXIT_SUCCESS;
    }
    else
    {
        error = EXIT_FAILURE;
    }
    return error;
}

int init_analisi_lexic(char *file_name)
{
    int error;

    yyin = fopen(file_name, "r");

    if (yyin == NULL)
    {
        error = EXIT_FAILURE;
    }
    else
    {
        error = EXIT_SUCCESS;
    }
    return error;
}

int end_analisi_lexic()
{
    int error;

    error = fclose(yyin);

    if (error == 0)
    {
        error = EXIT_SUCCESS;
    }
    else
    {
        error = EXIT_FAILURE;
    }
    return error;
}

void yyerror(char *explanation)
{
    fprintf(stderr, "Error: %s ,in line %d \n", explanation, yylineno);
}

void debug(char *text, char *var)
{
    printf(text, var);
}

void simpleDebug(char *text)
{
    printf(text);
}

int doOperationAritmeticP3(value_info v1, char *operand, value_info v2, value_info *finish_val)
{
    if (strcmp(v1.type, "Int32") == 0 && strcmp(v2.type, "Int32") == 0)
    {

        finish_val->type = "Int32";
        int resInt;
        debug("operand: %s\n", operand);
        if (intOperations(atoi(v1.value), atoi(v2.value), operand, &resInt))
        {
            sprintf(finish_val->value, "%d", resInt);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        finish_val->type = "Float64";
        float resFloat;
        if (floatOperations(atof(v1.value), atof(v2.value), operand, &resFloat))
        {
            sprintf(finish_val->value, "%f", resFloat);
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int canTypeDoOperationP3(char *type)
{
    return (strcmp(type, "Int32") == 0 || strcmp(type, "Float64") == 0);
}

int intOperations(int num1, int num2, char *operand, int *res)
{
    debug("num1: %s\n", iota(num1));
    debug("num2: %s\n", iota(num2));
    debug("operand: %s\n", operand);
    if (strcmp(operand, "+") == 0)
    {
        simpleDebug("Estoy en suma\n");
        *res = num1 + num2;
    }
    else if (strcmp(operand, "-") == 0)
    {
        simpleDebug("Estoy en resta\n");
        *res = num1 - num2;
    }
    else if (strcmp(operand, "*") == 0)
    {
        simpleDebug("Estoy en producto\n");
        *res = num1 * num2;
    }
    else if (strcmp(operand, "/") == 0)
    {
        simpleDebug("Estoy en division\n");
        if (num2 != 0)
        {
            *res = num1 / num2;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

float floatOperations(float num1, float num2, char *operand, float *res)
{
    if (strcmp(operand, "+") == 0)
    {
        *res = num1 + num2;
    }
    else if (strcmp(operand, "-") == 0)
    {
        *res = num1 - num2;
    }
    else if (strcmp(operand, "*") == 0)
    {
        *res = num1 * num2;
    }
    else if (strcmp(operand, "/") == 0)
    {
        if (num2 != 0)
        {
            *res = num1 / num2;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

char *iota(int num)
{
    char *string = (char *)malloc(sizeof(char) * STR_MAX_LENGTH);
    sprintf(string, "%i", num);
    return string;
}

char *fota(float num)
{
    char *string = (char *)malloc(sizeof(char) * STR_MAX_LENGTH);
    sprintf(string, "%f", num);
    return string;
}

char *strncpyWithSentinel(int length, char *toCopy)
{
    char *aux = (char *)malloc(sizeof(char) * (length + 1));
    strncpy(aux, toCopy, length);
    aux[length] = '\0';
    return aux;
}
