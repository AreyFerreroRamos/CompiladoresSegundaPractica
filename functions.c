#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
	exit(EXIT_FAILURE);
}

void debug(char *text, char *var)
{
	printf(text, var);
}

void simpleDebug(char *text)
{
	printf(text);
}

int doOperationAritmetic(value_info v1, char *operand, value_info v2, value_info *finish_val)
{
	if (strcmp(v1.type, "Int32") == 0 && strcmp(v2.type, "Int32") == 0)
	{
		finish_val->type = "Int32";
		int resInt;
		//debug("operand: %s\n", operand);
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
		if (strcmp(operand, "%") == 1)
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
		else
		{
			return 0;
		}
	}
	return 1;
}

int isNumberType(char *type)
{
	return (strcmp(type, INT32_T) == 0 || strcmp(type, FLOAT64_T) == 0);
}

int intOperations(int num1, int num2, char *operand, int *res)
{
	debug("num1: %s\n", iota(num1));
	debug("num2: %s\n", iota(num2));
	debug("operand: %s\n", operand);
	if (strcmp(operand, OP_ARIT_SUMA) == 0)
	{
		//simpleDebug("Estoy en suma\n");
		*res = num1 + num2;
	}
	else if (strcmp(operand, OP_ARIT_RESTA) == 0)
	{
		//simpleDebug("Estoy en resta\n");
		*res = num1 - num2;
	}
	else if (strcmp(operand, OP_ARIT_MULT) == 0)
	{
		//simpleDebug("Estoy en producto\n");
		*res = num1 * num2;
	}
	else if (strcmp(operand, OP_ARIT_DIV) == 0)
	{
		//simpleDebug("Estoy en division\n");
		if (num2 != 0)
		{
			*res = num1 / num2;
		}
		else
		{
			return 0;
		}
	}
	else if (strcmp(operand, OP_ARIT_MOD) == 0)
	{
		//simpleDebug("Estoy en modulo\n");
		if (num2 != 0)
		{
			*res = num1 % num2;
		}
		else
		{
			return 0;
		}
	}
	else if (strcmp(operand, OP_ARIT_POTENCIA) == 0)
	{
		//simpleDebug("Estoy en la potencia\n");
		*res = (int)pow((double)num1, (double)num2);
	}
	return 1;
}

float floatOperations(float num1, float num2, char *operand, float *res)
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
			return 0;
		}
	}
	else if (strcmp(operand, OP_ARIT_POTENCIA) == 0)
	{
		*res = (float)pow((double)num1, (double)num2);
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

value_info createValueInfo(int length, char *value, char *type)
{
	value_info aux;
	aux.value = (char *)malloc(sizeof(char) * length);
	sprintf(aux.value, "%s", value);
	aux.type = (char *)malloc(sizeof(char) * strlen(type));
	aux.type = type;
	return aux;
}

int isSameType(char *type1, char *type2)
{
	return strcmp(type1, type2) == 0;
}

int doRelationalOperation(float num1, char *op, float num2)
{
	debug("%s\n", fota(num1));
	debug("%s\n", op);
	debug("%s\n", fota(num2));
	if (strcmp(op, OP_REL_HIGH) == 0)
	{
		simpleDebug("Estoy en >\n");
		return num1 > num2;
	}
	if (strcmp(op, OP_REL_HE) == 0)
	{
		simpleDebug("Estoy en >=\n");
		return num1 >= num2;
	}
	if (strcmp(op, OP_REL_LESS) == 0)
	{
		simpleDebug("Estoy en <\n");
		return num1 < num2;
	}
	if (strcmp(op, OP_REL_LE) == 0)
	{
		simpleDebug("Estoy en <=\n");
		return num1 <= num2;
	}
	if (strcmp(op, OP_REL_EQUAL) == 0)
	{
		simpleDebug("Estoy en ==\n");
		return num1 == num2;
	}
	if (strcmp(op, OP_REL_DIFF) == 0)
	{
		simpleDebug("Estoy en !=\n");
		return num1 != num2;
	}
}

char *allocateSpaceForMessage()
{
	char *message;
	message = (char *)malloc(sizeof(char) * STR_MAX_LENGTH);
	return message;
}

int negateBoolean(int boolean)
{
	if (boolean == 0)
	{
		return 1;
	}
	return 0;
}

int isTrue(int boolean)
{
	return boolean == 1;
}
int isFalse(int boolean)
{
	return boolean == 0;
}
