#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int lengthCode;
extern char **c3a;

//-------------------------EMET FUNCTIONS----------------------------

char *emetCopy(value_info v1, value_info v2)
{
    char *instr;
        // Añadimos v1.
    instr = addV1(v1);

        // Añadimos primer operando.
    instr = addV2(instr, v2);

    return instr;
}

char *emetOperation(char *op, value_info v1, value_info v2, value_info v3)
{
    char *instr;
        // Añadimos v1.
    instr = addV1(v1);

        // Añadimos primer operando.
    instr = addV2(instr, v2);

        // Añadimos operación.
    instr = addOperation(instr, op);

        // Añadimos segundo operando.
    instr = addV3(instr, v3);

    return instr;
}

char *addV1(value_info v1)
{
    char *instr;
    // Añadimos v1
    if (isSameType(v1.valueInfoType, TENS_T))
    {
        instr = generateString("%s[%s] := ", 2, v1.value, v1.index.value);
    }
    else
    {
        instr = generateString("%s := ", 1, v1.value);
    }
    return strdup(instr);
}

char *addV2(char *instruction, value_info v2)
{
    char *instr = strdup(instruction);
    if (isSameType(v2.valueInfoType, VAR_T))
    {
        instr = generateString("%s%s ", 2, instr, v2.value);
    }
    else if (isSameType(v2.valueInfoType, TENS_T))
    {
        instr = generateString("%s%s[%s] ", 3, instr, v2.value, v2.index);
    }
    else if (isSameType(v2.valueInfoType, LIT_T))
    {
        instr = generateString("%s%s ", 2, instr, v2.value);
    }
    else if (isSameType(v2.valueInfoType, FUNC_T))
    {
        // var[9999999999] := CALL func, numArgs
        int numParams = 1;
        instr = generateString("%sCALL %s,%s", 3, instr, v2.value, itos(numParams));
        // TODO añadir parámetros de la función
    }
    return strdup(instr);
}

char *addOperation(char *instruction, char *op)
{
    return strdup(generateString("%s%s ", 2, instruction, op));
}

char *addV3(char *instruction, value_info v3)
{
    char *instr = strdup(instruction);
    if (isSameType(v3.valueInfoType, VAR_T))
    {
        instr = generateString("%s%s", 2, instr, v3.value);
    }
    else if (isSameType(v3.valueInfoType, TENS_T))
    {
        instr = generateString("%s%s[%s]", 3, instr, v3.value, v3.index);
    }
    else if (isSameType(v3.valueInfoType, LIT_T))
    {
        instr = generateString("%s%s", 2, instr, v3.value);
    }
    return strdup(instr);
}

//-------------------------------------------------------------------

void printCode3Adresses()
{
    printf("---------------------------------\n");
    for (int i = 0; i < lengthCode; i++)
    {
        printf("%s", c3a[i]);
        printf("\n");
        fprintf(yyout, "%s", c3a[i]);
    }
    printf("---------------------------------\n");
}

void intOperations(int num1, int num2, char *operand, int *res)
{
    if (strcmp(operand, OP_ARIT_SUMA) == 0)
    {
        simpleDebug("Estoy en suma.\n", 1);
        *res = num1 + num2;
    }
    else if (strcmp(operand, OP_ARIT_RESTA) == 0)
    {
        simpleDebug("Estoy en resta.\n", 1);
        *res = num1 - num2;
    }
    else if (strcmp(operand, OP_ARIT_MULT) == 0)
    {
        simpleDebug("Estoy en producto.\n", 1);
        *res = num1 * num2;
    }
    else if (strcmp(operand, OP_ARIT_DIV) == 0)
    {
        simpleDebug("Estoy en division.\n", 1);
        if (num2 != 0)
        {
            *res = num1 / num2;
        }
        else
        {
            yyerror("No esta permitido dividir por 0.");
        }
    }
    else if (strcmp(operand, OP_ARIT_MOD) == 0)
    {
        simpleDebug("Estoy en modulo\n", 1);
        if (num2 != 0)
        {
            *res = num1 % num2;
        }
        else
        {
            yyerror("No esta permitido dividir por 0.");
        }
    }
    else if (strcmp(operand, OP_ARIT_POTENCIA) == 0)
    {
        simpleDebug("Estoy en la potencia.\n", 1);
        *res = (int) pow((double) num1, (double) num2);
    }
}

void floatOperations(float num1, float num2, char *operand, float *res)
{
    if (strcmp(operand, OP_ARIT_SUMA) == 0)
    {
        *res = num1 + num2;
    }
    else if (strcmp(operand, OP_ARIT_RESTA) == 0)
    {
        *res = num1 - num2;
    }
    else if (strcmp(operand, OP_ARIT_MULT) == 0)
    {
        *res = num1 * num2;
    }
    else if (strcmp(operand, OP_ARIT_DIV) == 0)
    {
        if (num2 != 0)
        {
            *res = num1 / num2;
        }
        else
        {
            yyerror("No esta permitido dividir por 0.");
        }
    }
    else if (strcmp(operand, OP_ARIT_POTENCIA) == 0)
    {
        *res = (float) pow((double) num1, (double) num2);
    }
}

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

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
