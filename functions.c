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

value_info createValueInfo(char *value, char *type, char *lexema)
{
	value_info aux;
	if (value != NULL)
	{
		aux.value = strdup(value);
	}
	else
	{
		aux.value = NULL;
	}
	aux.type = strdup(type);
	if (lexema != NULL)
	{
		aux.lexema = strdup(lexema);
	}
	else
	{
		aux.lexema = NULL;
	}
	return aux;
}

tensor_info createTensorInfo(int index_dim, int calcIndex, char *lexema)
{
	tensor_info aux;

	aux.index_dim = index_dim;
	aux.calcIndex = calcIndex;
	if (lexema != NULL)
	{
		aux.lexema = strdup(lexema);
	}
	else
	{
		aux.lexema = NULL;
	}
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
			return entry.elem_dims[index_dim];
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
		int i = num_element1;
		for (int j = 0; j < num_element2; j++)
		{
			((int *)ptr)[i++] = ((int *)elements2)[j];
		}
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

int doTensorCalcs(char *nameVar1, char *nameVar2, char *operation)
{
	// Si las dos variables son tensores
	if (nameVar1 != NULL && nameVar2 != NULL)
	{
		sym_value_type entry1;
		int response1 = sym_lookup(nameVar1, &entry1);
		sym_value_type entry2;
		int response2 = sym_lookup(nameVar2, &entry2);
		// Si las dos variables existen
		if (response1 == SYMTAB_OK && response2 == SYMTAB_OK)
		{
			// Si los dos tensores tienen el mismo numero de dimensiones
			if (entry1.num_dim == entry2.num_dim)
			{
				// Si los dos tensores tienen el mismo numero de delementos en cada dimension
				for (int i = 0; i < entry1.num_dim; i++)
				{
					if (entry1.elem_dims[i] != entry2.elem_dims[i])
					{
						return -5;
					}
				}
				sym_value_type tmp;
				tmp.num_dim = entry1.num_dim;
				tmp.value = NULL;
				// Si tienen el mismo tamaño (son del mismo tipo)
				if (entry1.size == entry2.size)
				{
					tmp.size = entry1.size;
				}
				else
				{
					if (maxNum((float)entry1.size, (float)entry2.size))
					{
						tmp.size = entry1.size;
					}
					else
					{
						tmp.size = entry2.size;
					}
				}
				// Si alguno de los dos tipos es float
				if (isSameType(entry1.type, FLOAT64_T) || isSameType(entry2.type, FLOAT64_T))
				{
					tmp.type = FLOAT64_T;
				}
				else
				{
					tmp.type = INT32_T;
				}
				tmp.elements = malloc(tmp.size);
				tmp.elem_dims = entry1.elem_dims;

				// Por cada elemento hacemos operación
				int cont = tmp.size / calculateSizeType(tmp.type);
				for (int i = 0; i < cont; i++)
				{
					value_info v1;
					if (isSameType(entry1.type, INT32_T))
					{
						v1.value = iota(((int *)entry1.elements)[i]);
					}
					else if (isSameType(entry1.type, FLOAT64_T))
					{
						v1.value = fota(((float *)entry1.elements)[i]);
					}
					v1.lexema = nameVar1;
					v1.type = entry1.type;
					value_info v2;
					if (isSameType(tmp.type, INT32_T))
					{
						v2.value = iota(((int *)entry2.elements)[i]);
					}
					else if (isSameType(tmp.type, FLOAT64_T))
					{
						v2.value = fota(((float *)entry2.elements)[i]);
					}
					v2.lexema = nameVar2;
					v2.type = entry2.type;
					value_info aux;
					doAritmeticOperation(v1, operation, v2, &aux);
					if (isSameType(tmp.type, INT32_T))
					{
						((int *)tmp.elements)[i] = atoi(aux.value);
					}
					else if (isSameType(tmp.type, FLOAT64_T))
					{
						((float *)tmp.elements)[i] = atof(aux.value);
					}
				}
				int message = sym_enter(TMP_FOR_TENSOR_RESULT, &tmp);
				if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE)
				{
					return -5;
				}
				return 0;
			}
			else
			{
				return -4;
			}
		}
		else
		{
			return -3;
		}
	}
	// Si las dos variables son numeros
	if (nameVar1 == NULL && nameVar2 == NULL)
	{
		return -2;
	}
	return -1;
}

int doTensorProduct(char *nameVar1, char *nameVar2, char *operation)
{
	sym_value_type tmp;
	// Si las dos variables son tensores
	if (nameVar1 != NULL && nameVar2 != NULL)
	{
		sym_value_type entry1;
		int response1 = sym_lookup(nameVar1, &entry1);
		sym_value_type entry2;
		int response2 = sym_lookup(nameVar2, &entry2);
		// Si las dos variables existen
		int response = isPossibleTensorProduct(entry1.elem_dims, entry1.num_dim, entry2.elem_dims, entry2.num_dim);
		if (response == 0)
		{
			// Los tensores se pueden multiplicar
			if (entry1.num_dim == 1 || entry2.num_dim == 1)
			{
				tmp.num_dim = 1;
			}
			else
			{
				tmp.num_dim = 2;
			}
			tmp.value = NULL;
			if (isSameType(entry1.type, FLOAT64_T) || isSameType(entry2.type, FLOAT64_T))
			{
				tmp.type = FLOAT64_T;
			}
			else
			{
				tmp.type = INT32_T;
			}
			int nRowsMatrix1, nColsMatrix2;
			if (entry1.num_dim == 1)
			{
				nRowsMatrix1 = 1;
			}
			else
			{
				nRowsMatrix1 = entry1.elem_dims[1];
			}
			if (entry2.num_dim == 1)
			{
				nColsMatrix2 = 1;
			}
			else
			{
				nColsMatrix2 = entry2.elem_dims[0];
			}
			tmp.size = nRowsMatrix1 * nColsMatrix2 * calculateSizeType(tmp.type);
			tmp.elements = malloc(tmp.size);
			tmp.elem_dims = malloc(sizeof(int) * tmp.num_dim);
			if (tmp.num_dim == 1)
			{
				tmp.elem_dims[0] = maxNum((float)nRowsMatrix1, (float)nColsMatrix2);
			}
			else
			{
				tmp.elem_dims[0] = nRowsMatrix1;
				tmp.elem_dims[1] = nColsMatrix2;
			}
			// MUTLIPLICACION DE DOS TENSORES
		}
		else
		{
			return response;
		}
	}
	// Si las dos variables son numeros
	else if (nameVar1 == NULL && nameVar2 == NULL)
	{
		return -4;
	}
	// Si una es valor i la otra es un tensor
	else
	{
		// HACER VALOR POR CADA UNO DE LOS VALORES DEL TENSOR
		return -3;
	}
}

int doNumberProductTensor(char *number, char type, char *nameTensor)
{
	sym_value_type entry;
	int response1 = sym_lookup(nameTensor, &entry);
	sym_value_type tmp = entry;
	if (isSameType(type, FLOAT64_T) || isSameType(entry.type, FLOAT64_T))
	{
		if (isSameType(type, FLOAT64_T) && isSameType(entry.type, FLOAT64_T))
		{
			for (int i = 0; i < (entry.size / calculateSizeType(entry.type)); i++)
			{
				((float *)tmp.elements)[i] = (float)((float *)entry.elements)[i] * atof(number);
			}
		}
		else if (isSameType(type, FLOAT64_T) && isSameType(entry.type, INT32_T))
		{
			for (int i = 0; i < (entry.size / calculateSizeType(entry.type)); i++)
			{
				((float *)tmp.elements)[i] = (float)((int *)entry.elements)[i] * atof(number);
			}
		}
		else
		{
			for (int i = 0; i < (entry.size / calculateSizeType(entry.type)); i++)
			{
				((float *)tmp.elements)[i] = (float)((float *)entry.elements)[i] * atoi(number);
			}
		}
	}
	else
	{
		for (int i = 0; i < (entry.size / calculateSizeType(entry.type)); i++)
		{
			((int *)tmp.elements)[i] = (int)((int *)entry.elements)[i] * atoi(number);
		}
	}
	int message = sym_enter(TMP_FOR_TENSOR_RESULT, &tmp);
	if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE)
	{
		return -1;
	}
	return 0;
}

int doTensorProductTensor(char *nameVar1, char *nameVar2, sym_value_type tmp)
{
	sym_value_type matrix1, matrix2;
	int response = sym_lookup(nameVar1, &matrix1);
	if (response == SYMTAB_OK)
	{
		int response = sym_lookup(nameVar2, &matrix2);
		if (response == SYMTAB_OK)
		{
			int rowsM1, colsM1, rowsM2, colsM2;
			if (matrix1.num_dim == 1)
			{
				rowsM1 = 1;
				colsM1 = matrix1.elem_dims[0];
			}
			else
			{
				rowsM1 = matrix1.elem_dims[0];
				colsM1 = matrix1.elem_dims[1];
			}
			if (matrix2.num_dim == 1)
			{
				rowsM2 = matrix2.elem_dims[0];
				colsM2 = 1;
			}
			else
			{
				rowsM2 = matrix2.elem_dims[0];
				colsM2 = matrix2.elem_dims[1];
			}
			for (int i = 0; i < rowsM1; i++)
			{
				for (int j = 0; j < colsM2; j++)
				{
					if (isSameType(tmp.type, INT32_T))
					{
						((int *)tmp.elements)[i * colsM2 + j] = 0;
					}
					else
					{
						((float *)tmp.elements)[i * colsM2 + j] = 0;
					}
					sym_value_type aux;
					aux.elements = malloc(tmp.size);
					for (int k = 0; k < colsM1; k++)
					{
						value_info v1;
						v1.lexema = nameVar1;
						v1.type = matrix1.type;
						v1.value = i * colsM1 + k;
						value_info v2;
						v2.lexema = nameVar2;
						v2.type = matrix2.type;
						v2.value = k * colsM2 + j;
						asignacionTensor(&aux, i * colsM1 + j, v1, v2, "+");
					}
				}
			}
		}
		else
		{
			// NO EXISTE NAMEVAR2
			return -2;
		}
	}
	// NO EXISTE NAMEVAR1
	return -1;
}

int asignacionTensor(sym_value_type *result, int posicion, value_info v1, value_info v2, char *op)
{
	sym_value_type entry1, entry2;
	if (v1.lexema != NULL)
	{
		int response = sym_lookup(v1.lexema, &entry1);
		if (response == SYMTAB_OK)
		{
			int aux = atoi(v1.value);
			v1.type = entry1.type;
			if (isSameType(entry1.type, INT32_T))
			{
				v1.value = iota(((int *)entry1.elements)[aux]);
			}
			else
			{
				v1.value = fota(((float *)entry1.elements)[aux]);
			}
		}
		else
		{
			return -1;
		}
	}
	if (v2.lexema != NULL)
	{
		int response = sym_lookup(v2.lexema, &entry2);
		if (response == SYMTAB_OK)
		{
			int aux = atoi(v2.value);
			v2.type = entry2.type;
			if (isSameType(entry2.type, INT32_T))
			{
				v2.value = iota(((int *)entry2.elements)[aux]);
			}
			else
			{
				v2.value = fota(((float *)entry2.elements)[aux]);
			}
		}
		else
		{
			return -1;
		}
	}
	if (isSameType(result->type, INT32_T))
	{
		value_info finishVal;
		doAritmeticOperation(v1, op, v2, &finishVal);
		if (isSameType(finishVal.type, INT32_T))
		{
			((int *)result->elements)[posicion] = atoi(finishVal.value);
		}
		else
		{
			((int *)result->elements)[posicion] = (int)atof(finishVal.value);
		}
	}
	else
	{
		value_info finishVal;
		doAritmeticOperation(v1, op, v2, &finishVal);
		if (isSameType(finishVal.type, INT32_T))
		{
			((float *)result->elements)[posicion] = (float)atoi(finishVal.value);
		}
		else
		{
			((float *)result->elements)[posicion] = atof(finishVal.value);
		}
	}
	return 0;
}

int isPossibleTensorProduct(int *elemDims1, int numDims1, int *elemDims2, int numDims2)
{
	// Si son matrices o vectores
	if (numDims1 <= 2 && numDims2 <= 2)
	{
		int nColsMatrix1, nRowsMatrix2;
		if (numDims1 == 1)
		{
			nColsMatrix1 = elemDims1[0];
		}
		else
		{
			nColsMatrix1 = elemDims1[1];
		}
		if (numDims2 == 1)
		{
			nRowsMatrix2 = elemDims2[0];
		}
		else
		{
			nRowsMatrix2 = elemDims2[0];
		}
		if (nColsMatrix1 == nRowsMatrix2)
		{
			return 0;
		}
		return -1;
	}
	return -2;
}

int maxNum(float a, float b)
{
	if (a > b)
	{
		return 1;
	}
	else if (a == b)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void printTensor(char *nameVar, sym_value_type tensor, int inFile)
{
	if (inFile)
	{
		fprintf(yyout, "%s[", nameVar);
		for (int i = 0; i < (tensor.size / calculateSizeType(tensor.type)); i++)
		{
			if (isSameType(tensor.type, INT32_T))
			{
				fprintf(yyout, "%i,", ((int *)tensor.elements)[i]);
			}
			else if (isSameType(tensor.type, FLOAT64_T))
			{
				fprintf(yyout, "%f,", ((float *)tensor.elements)[i]);
			}
		}
		fprintf(yyout, "]\n");
	}
	else
	{
		printf("%s[", nameVar);
		for (int i = 0; i < (tensor.size / calculateSizeType(tensor.type)); i++)
		{
			if (isSameType(tensor.type, INT32_T))
			{
				printf("%i,", ((int *)tensor.elements)[i]);
			}
			else if (isSameType(tensor.type, FLOAT64_T))
			{
				printf("%f,", ((float *)tensor.elements)[i]);
			}
		}
		printf("]\n");
	}
}

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
