#include <stdio.h>
#include <stdlib.h>

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
