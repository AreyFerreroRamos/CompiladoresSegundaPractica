#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;

//FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

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

void yyerror(char *explanation)
{
	fprintf(stderr, "Error: %s ,in line %d \n", explanation, yylineno);
	exit(EXIT_FAILURE);
}

// UTILS

void debug(char *text, char *var, int typeFile)
{
	//flex
	if (typeFile == 0)
	{
		printf(text, var);
	}
	//bison
	else
	{
		//printf(text, var);
	}
}

void simpleDebug(char *text, int typeFile)
{
	//flex
	if (typeFile == 0)
	{
		//printf(text);
	}
	//bison
	else
	{
		//printf(text);
	}
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

value_info createValueInfo(int length, char *value, char *type)
{
	value_info aux;
	aux.value = strdup(value);
	aux.type = strdup(type);
	return aux;
}

tensor_info createTensorInfo(int dim, int calcIndex, char *lexema)
{
	tensor_info aux;
	aux.dim = dim;
	aux.calcIndex = calcIndex;
	aux.lexema = strdup(lexema);
	return aux;
}

char *getIdName(char *idWithAssign)
{
	int sentinel;
	for (int i = 0; i < strlen(idWithAssign); i++)
	{
		if (idWithAssign[i] == ' ' || idWithAssign[i] == '\t' || idWithAssign[i] == '=')
		{
			sentinel = i;
			break;
		}
	}
	char *var = allocateSpaceForMessage(STR_MAX_LENGTH);
	strncpy(var, idWithAssign, sentinel);
	return var;
}

int getDim(char *key, int dim)
{
	sym_value_type entry;
	int error = sym_lookup(key, &entry);
	if (error == SYMTAB_OK)
	{
		if (dim < entry.num_dim)
		{
			return entry.elem_dims[dim - 1];
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

//CONTROLS

int isSameType(char *type1, char *type2)
{
	return strcmp(type1, type2) == 0;
}

int isNumberType(char *type)
{
	return (strcmp(type, INT32_T) == 0 || strcmp(type, FLOAT64_T) == 0);
}

//OPERATIONS

int doAritmeticOperation(value_info v1, char *operand, value_info v2, value_info *finish_val)
{
	if (strcmp(v1.type, "Int32") == 0 && strcmp(v2.type, "Int32") == 0)
	{
		finish_val->type = "Int32";
		int resInt;
		if (intOperations(atoi(v1.value), atoi(v2.value), operand, &resInt))
		{
			finish_val->value = iota(resInt);
		}
		else
		{
			simpleDebug("int", 1);
			return 0;
		}
	}
	else
	{
		if (strcmp(operand, "%") != 0)
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

int doRelationalOperation(float num1, char *op, float num2)
{
	debug("%s\n", fota(num1), 1);
	debug("%s\n", op, 1);
	debug("%s\n", fota(num2), 1);
	if (strcmp(op, OP_REL_HIGH) == 0)
	{
		simpleDebug("Estoy en >\n", 1);
		return num1 > num2;
	}
	if (strcmp(op, OP_REL_HE) == 0)
	{
		simpleDebug("Estoy en >=\n", 1);
		return num1 >= num2;
	}
	if (strcmp(op, OP_REL_LESS) == 0)
	{
		simpleDebug("Estoy en <\n", 1);
		return num1 < num2;
	}
	if (strcmp(op, OP_REL_LE) == 0)
	{
		simpleDebug("Estoy en <=\n", 1);
		return num1 <= num2;
	}
	if (strcmp(op, OP_REL_EQUAL) == 0)
	{
		simpleDebug("Estoy en ==\n", 1);
		return num1 == num2;
	}
	if (strcmp(op, OP_REL_DIFF) == 0)
	{
		simpleDebug("Estoy en !=\n", 1);
		return num1 != num2;
	}
}

int intOperations(int num1, int num2, char *operand, int *res)
{
	if (strcmp(operand, OP_ARIT_SUMA) == 0)
	{
		simpleDebug("Estoy en suma\n", 1);
		*res = num1 + num2;
	}
	else if (strcmp(operand, OP_ARIT_RESTA) == 0)
	{
		simpleDebug("Estoy en resta\n", 1);
		*res = num1 - num2;
	}
	else if (strcmp(operand, OP_ARIT_MULT) == 0)
	{
		simpleDebug("Estoy en producto\n", 1);
		*res = num1 * num2;
	}
	else if (strcmp(operand, OP_ARIT_DIV) == 0)
	{
		simpleDebug("Estoy en division\n", 1);
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
		simpleDebug("Estoy en modulo\n", 1);
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
		simpleDebug("Estoy en la potencia\n", 1);
		*res = (int)pow((double)num1, (double)num2);
	}
	return 1;
}

int floatOperations(float num1, float num2, char *operand, float *res)
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

int lenght(char *key)
{
	sym_value_type entry;
	int response = sym_lookup(key, &entry);
	if (response == SYMTAB_OK)
	{
		return entry.size;
	}
	else if (response = SYMTAB_NOT_FOUND)
	{
		return strlen(key);
	}
	else
	{
		return -1;
	}
}

void convert_invert_vector(char * vector, int dim)
{
	char * aux;
	int i;
	
	for (i = 0; i < dim - 1; i++) {
		vector[i] = vector[i] / vector[i + 1];
	}

	for (i = 0; i < dim / 2; i++)
	{
		*aux = vector[i];
		vector[i] = vector[dim - i - 1];
		vector[dim - i - 1] = *aux;
	}
}
