#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;

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

void yyerror(char *explanation)
{
	fprintf(stderr, "Error: %s ,in line %d \n", explanation, yylineno);
	exit(EXIT_FAILURE);
}

// FUNCIONES DE UTILIDAD

void debug(char *text, char *var, int typeFile)
{
	// flex
	if (typeFile == 0)
	{
		printf(text, var);
	}
	// bison
	else
	{
		// printf(text, var);
	}
}

void simpleDebug(char *text, int typeFile)
{
	// flex
	if (typeFile == 0)
	{
		// printf(text);
	}
	// bison
	else
	{
		// printf(text);
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

tensor_info createTensorInfo(int index_dim, int calcIndex, char *lexema)
{
	tensor_info aux;

	aux.index_dim = index_dim;
	aux.calcIndex = calcIndex;
	aux.lexema = strdup(lexema);
	return aux;
}

char *getIdName(char *idWithAssign)
{
	int sentinel, i;

	for (i = 0; i < strlen(idWithAssign); i++)
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

int getDim(char *key, int index_dim)
{
	sym_value_type entry;
	int error = sym_lookup(key, &entry);

	if (error == SYMTAB_OK)
	{
		if (index_dim < entry.num_dim)
		{
			return entry.elem_dims[index_dim - 1];
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

int *convert_invert_vector(int *vector, int dim)
{

	int aux;
	int i;

	for (i = 0; i < dim - 1; i++) // Convertir el vector con el número de elementos totales por dimensión en un vector con el número de elementos de la siguiente dimensión.
	{
		vector[i] = vector[i] / vector[i + 1];
	}

	for (i = 0; i < dim / 2; i++) // Invertir el vector.
	{
		aux = vector[i];
		vector[i] = vector[dim - i - 1];
		vector[dim - i - 1] = aux;
	}
	return vector;
}

void castValueToVoidPointer(void *ptr, char *value, char *type)
{
	if (isSameType(type, INT32_T))
	{
		((int *)ptr)[0] = atoi(value);
	}
	else if (isSameType(type, FLOAT64_T))
	{
		((float *)ptr)[0] = atof(value);
	}
}

void castTensorToVoidPointer(void *ptr, void *elements1, char *type1, int num_element1, void *elements2, char *type2, int num_element2)
{
	char *typefinal;
	if (isSameType(type1, INT32_T) && isSameType(type2, INT32_T))
	{
		typefinal = INT32_T;
	}
	else
	{
		typefinal = FLOAT64_T;
	}
	// Si la lista final se trata como entera
	if (isSameType(typefinal, INT32_T))
	{
		int i;
		printf("---");
		for (i = 0; i < num_element1; i++)
		{
			printf("%i ", ((int *)elements1)[i]);
			((int *)ptr)[i] = ((int *)elements1)[i];
		}

		for (int j = 0; j < num_element2; j++)
		{
			printf("%i ", ((int *)elements2)[j]);
			((int *)ptr)[i++] = ((int *)elements2)[j];
		}
		printf("---\n");
	}
	// Si la lista final se trata como float
	else
	{
		int i;
		// Si los valores se tratan como enteros
		if (isSameType(type1, INT32_T))
		{
			for (i = 0; i < num_element1; i++)
			{
				((float *)ptr)[i] = (float)((int *)elements1)[i];
			}
		}
		// Si los valores se tratan como float
		else if (isSameType(type1, FLOAT64_T))
		{
			for (i = 0; i < num_element1; i++)
			{
				((float *)ptr)[i] = ((float *)elements1)[i];
			}
		}
		// Si los valores se tratan como enteros
		if (isSameType(type2, INT32_T))
		{
			for (int j = i; j < num_element1 + num_element2; j++)
			{
				((float *)ptr)[j] = (float)((int *)elements2)[j];
			}
		}
		// Si los valores se tratan como float
		else if (isSameType(type2, FLOAT64_T))
		{
			for (int j = i; j < num_element1 + num_element2; j++)
			{
				((float *)ptr)[j] = ((float *)elements2)[j];
			}
		}
	}
}

int calculateSizeType(char *type)
{
	if (isSameType(type, FLOAT64_T))
	{
		return 8;
	}
	else if (isSameType(type, INT32_T))
	{
		return 4;
	}
	else
	{
		return 1;
	}
}

/*void addElementsDim(int *vector_dims_tensor, int index, int *num_dims)
{
	printf("addElementDim1: num_dims:%i\n", *num_dims);

	printf("addElementDim2: num elem: %i en dim: %i", vector_dims_tensor[0], index);

	vector_dims_tensor[index] += 1;

	printf("addElementDim3: num_dims:%i\n", *num_dims);
}*/

// FUNCIONES DE CONTROL DE ERRORES

int isSameType(char *type1, char *type2)
{
	return (strcmp(type1, type2) == 0);
}

int isNumberType(char *type)
{
	return (strcmp(type, INT32_T) == 0 || strcmp(type, FLOAT64_T) == 0);
}

// FUNCIONES PARA REALIZAR OPERACIONES

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
