#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;

extern char **list_tmp_variables_symtab;
extern int num_tmp_variable;

// FUNCIONES DE UTILIDAD

int negateBoolean(int boolean)
{
	if (boolean == 0)
	{
		return 1;
	}
	return 0;
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
	char *var = allocateSpaceForMessage(ID_MAX_LENGTH);
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

void castTensorToVoidPointer(void *ptr, char *type1, int num_element1, void *elements2, char *type2, int num_element2)
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
	if (isSameType(typefinal, INT32_T))
	{ // Si la lista final se trata como entera.
		int i = num_element1;
		for (int j = 0; j < num_element2; j++)
		{
			((int *)ptr)[i++] = ((int *)elements2)[j];
		}
	}
	else
	{ // Si la lista final se trata como float.
		int i = num_element1;
		if (isSameType(type1, INT32_T))
		{
			for (int j = 0; j < i; j++)
			{
				((float *)ptr)[j] = ((int *)ptr)[j];
			}
		}
		if (isSameType(type2, INT32_T))
		{ // Si los valores se tratan como enteros.
			for (int j = 0; j < num_element2; j++)
			{
				((float *)ptr)[i++] = ((int *)elements2)[j];
			}
		}
		else if (isSameType(type2, FLOAT64_T))
		{ // Si los valores se tratan como float.
			for (int j = 0; j < num_element2; j++)
			{
				((float *)ptr)[i++] = ((float *)elements2)[j];
			}
		}
	}
}

void asignacionTensor(sym_value_type *result, int posicion, value_info v1, value_info v2, char *op)
{

	sym_value_type entry1, entry2;
	if (v1.lexema != NULL)
	{
		sym_lookup(v1.lexema, &entry1);
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
	if (v2.lexema != NULL)
	{
		sym_lookup(v2.lexema, &entry2);
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
	value_info finishVal;
	doAritmeticOperation(v1, op, v2, &finishVal);
	if (isSameType(result->type, INT32_T))
	{
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
		if (isSameType(finishVal.type, INT32_T))
		{
			((float *)result->elements)[posicion] = atoi(finishVal.value);
		}
		else
		{
			((float *)result->elements)[posicion] = atof(finishVal.value);
		}
	}
}

int isPossibleTensorProduct(int *elemDims1, int numDims1, int *elemDims2, int numDims2)
{
	if (numDims1 <= 2 && numDims2 <= 2)
	{ // Si son matrices o vectores.
		int nColsMatrix1, nRowsMatrix2;
		if (numDims1 == 1)
		{
			nColsMatrix1 = elemDims1[0];
		}
		else
		{
			nColsMatrix1 = elemDims1[1];
		}
		nRowsMatrix2 = elemDims2[0];
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

void printSymValueType(sym_value_type entry)
{
	printf("------------------------\n");
	printf("TYPE: %s\n", (char *)entry.type);
	printf("VALUE: %s\n", (char *)entry.value);
	printf("SIZE: %i\n", entry.size);
	printf("NUM DIM: %i\n", entry.num_dim);
	if (entry.num_dim > 0)
	{
		printf("ELEM DIMS: ");
		for (int i = 0; i < entry.num_dim; i++)
		{
			printf("%i ", entry.elem_dims[i]);
		}
		printf("\n");
	}
	printf("------------------------\n\n");
}

int getAcumElemDim(int *elem_dim, int num_dim)
{
	int acum = 1;
	for (int i = 0; i < num_dim; i++)
	{
		acum *= elem_dim[i];
	}
	return acum;
}

void saveTmpTensorInSymTab(value_info *val, char *type1, char *type2, sym_value_type entry)
{
	char *id = "prueba"; // generateTmpTensorId();
	int message = sym_enter(id, &entry);
	if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE)
	{
		yyerror("Error al guardar en symtab.");
	}
	else
	{
		if (isSameType(type1, FLOAT64_T) || isSameType(type2, FLOAT64_T))
		{
			val->type = FLOAT64_T;
		}
		else
		{
			val->type = INT32_T;
		}
		val->value = NULL;
		val->lexema = id;
	}
}

void clearTmpTensorId()
{
	for (int i = 0; i < num_tmp_variable; i++)
	{
		int response = sym_remove(list_tmp_variables_symtab[i]);
		if (response != SYMTAB_OK)
		{
			printf("ALGO MALO HA PASADO BORRANDO LAS VARIABLES.");
		}
	}
	free(list_tmp_variables_symtab);
	list_tmp_variables_symtab = NULL;
	num_tmp_variable = 0;
}

// FUNCIONES DE CONTROL DE ERRORES

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
	return 0;
}

int lenght(char *key)
{
	sym_value_type entry;
	int response = sym_lookup(key, &entry);

	if (response == SYMTAB_OK)
	{
		return entry.size;
	}
	else if (response == SYMTAB_NOT_FOUND)
	{
		return strlen(key);
	}
	else
	{
		return -1;
	}
}

int doTensorCalcs(char *nameVar1, char *nameVar2, char *operation, sym_value_type *tmp)
{
	if (nameVar1 != NULL && nameVar2 != NULL)
	{ // Si las dos variables son tensores.
		sym_value_type entry1;
		int response1 = sym_lookup(nameVar1, &entry1);
		sym_value_type entry2;
		int response2 = sym_lookup(nameVar2, &entry2);
		if (response1 == SYMTAB_OK && response2 == SYMTAB_OK)
		{ // Si las dos variables existen.
			if (entry1.num_dim == entry2.num_dim)
			{ // Si los dos tensores tienen el mismo numero de dimensiones.
				for (int i = 0; i < entry1.num_dim; i++)
				{ // Si los dos tensores tienen el mismo número de elementos en cada dimension.
					if (entry1.elem_dims[i] != entry2.elem_dims[i])
					{
						return -5;
					}
				}
				if (isSameType(entry1.type, FLOAT64_T) || isSameType(entry2.type, FLOAT64_T))
				{ // Si alguno de los dos tipos es float.
					tmp->type = FLOAT64_T;
				}
				else
				{
					tmp->type = INT32_T;
				}
				tmp->value = NULL;
				if (entry1.size == entry2.size)
				{ // Si tienen el mismo tamaño (son del mismo tipo).
					tmp->size = entry1.size;
				}
				else
				{
					if (maxNum((float)entry1.size, (float)entry2.size) == 1)
					{
						tmp->size = entry1.size;
					}
					else
					{
						tmp->size = entry2.size;
					}
				}
				tmp->num_dim = entry1.num_dim;
				tmp->elem_dims = entry1.elem_dims;
				tmp->elements = malloc(tmp->size);

				int cont = tmp->size / calculateSizeType(tmp->type);
				for (int i = 0; i < cont; i++)
				{ // Por cada elemento hacemos operación.
					value_info v1;
					v1.lexema = nameVar1;
					v1.type = entry1.type;
					v1.value = iota(i);
					value_info v2;
					v2.lexema = nameVar2;
					v2.type = entry2.type;
					v2.value = iota(i);
					asignacionTensor(&(*tmp), i, v1, v2, operation);
				}
				return 0;
			}
			else
			{
				// Tensores con diferentes dimensiones.
				return -4;
			}
		}
		else
		{
			// Alguna de las variables buscadas no existe.
			return -3;
		}
	}
	else if (nameVar1 == NULL && nameVar2 == NULL)
	{ // Si las dos variables son numeros.
		return -2;
	}
	else
	{ // Si una de las variables es un número y la otra un tensor.
		return -1;
	}
}

int doTensorProductInit(char *nameVar1, char *nameVar2, sym_value_type *tmp)
{
	if (nameVar1 != NULL && nameVar2 != NULL)
	{ // Si las dos variables son tensores.
		sym_value_type entry1;
		sym_lookup(nameVar1, &entry1);
		sym_value_type entry2;
		sym_lookup(nameVar2, &entry2);
		int response = isPossibleTensorProduct(entry1.elem_dims, entry1.num_dim, entry2.elem_dims, entry2.num_dim);
		if (response == 0)
		{ // Los tensores se pueden multiplicar.
			if (isSameType(entry1.type, FLOAT64_T) || isSameType(entry2.type, FLOAT64_T))
			{
				tmp->type = FLOAT64_T;
			}
			else
			{
				tmp->type = INT32_T;
			}
			tmp->value = NULL;
			int rowsM1, colsM2;
			if (entry1.num_dim == 1)
			{
				rowsM1 = 1;
			}
			else
			{
				rowsM1 = entry1.elem_dims[0];
			}
			if (entry2.num_dim == 1)
			{
				colsM2 = 1;
			}
			else
			{
				colsM2 = entry2.elem_dims[1];
			}
			tmp->num_dim = rowsM1 == 1 || colsM2 == 1 ? 1 : 2;
			if (tmp->num_dim == 1)
			{
				tmp->elem_dims = malloc(4);
				if (maxNum((float)rowsM1, (float)colsM2) == 1)
				{
					tmp->elem_dims[0] = rowsM1;
				}
				else
				{
					tmp->elem_dims[0] = colsM2;
				}
			}
			else
			{
				tmp->elem_dims = malloc(8);
				tmp->elem_dims[0] = rowsM1;
				tmp->elem_dims[1] = colsM2;
			}
			tmp->size = getAcumElemDim(tmp->elem_dims, tmp->num_dim) * calculateSizeType(tmp->type);
			tmp->elements = malloc(tmp->size);
			// MUTLIPLICACION DE DOS TENSORES.
			return 0;
		}
		else
		{
			return response;
		}
	}
	else if (nameVar1 == NULL && nameVar2 == NULL)
	{ // Si las dos variables son números.
		return -4;
	}
	else
	{ // Si una es valor i la otra es un tensor.
		// HACER VALOR POR CADA UNO DE LOS VALORES DEL TENSOR.
		return -3;
	}
}

int doNumberProductTensor(char *number, char *type, char *nameTensor, sym_value_type *tmp)
{
	sym_value_type entry;
	int response = sym_lookup(nameTensor, &entry);
	if (response == SYMTAB_OK)
	{
		*tmp = entry;
		for (int i = 0; i < (entry.size / calculateSizeType(entry.type)); i++)
		{
			value_info v1;
			v1.type = type;
			v1.value = number;
			v1.lexema = NULL;
			value_info v2;
			v2.type = entry.type;
			v2.value = iota(i);
			v2.lexema = nameTensor;
			asignacionTensor(&(*tmp), i, v1, v2, "*");
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int doTensorProductTensor(char *nameVar1, char *nameVar2, sym_value_type *tmp)
{
	sym_value_type matrix1, matrix2;
	int response = sym_lookup(nameVar1, &matrix1);
	if (response == SYMTAB_OK)
	{
		response = sym_lookup(nameVar2, &matrix2);
		if (response == SYMTAB_OK)
		{
			int rowsM1, colsM1, colsM2, rFinal, cFinal;
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
				colsM2 = 1;
			}
			else
			{
				colsM2 = matrix2.elem_dims[1];
			}
			sym_value_type aux;
			aux.elements = malloc(calculateSizeType(tmp->type));
			for (int i = 0; i < rowsM1; i++)
			{
				for (int j = 0; j < colsM2; j++)
				{ // Si transponemos el vector para poder multiplicarlo hay que trasponer los índices.
					rFinal = colsM2 == 1 ? j : i;
					cFinal = colsM2 == 1 ? i : j;
					if (isSameType(tmp->type, INT32_T))
					{
						((int *)aux.elements)[0] = 0;
					}
					else
					{
						((float *)aux.elements)[0] = 0;
					}
					for (int k = 0; k < colsM1; k++)
					{
						value_info v1;
						v1.type = matrix1.type;
						v1.value = iota(i * colsM1 + k);
						v1.lexema = nameVar1;
						value_info v2;
						v2.type = matrix2.type;
						v2.value = iota(k * colsM2 + j);
						v2.lexema = nameVar2;
						asignacionTensor(tmp, rFinal * colsM2 + cFinal, v1, v2, "*");
						if (isSameType(tmp->type, INT32_T))
						{
							((int *)aux.elements)[0] += ((int *)tmp->elements)[rFinal * colsM2 + cFinal];
						}
						else
						{
							((float *)aux.elements)[0] += ((float *)tmp->elements)[rFinal * colsM2 + cFinal];
						}
					}
					if (isSameType(tmp->type, INT32_T))
					{
						((int *)tmp->elements)[rFinal * colsM2 + cFinal] = ((int *)aux.elements)[0];
					}
					else
					{
						((float *)tmp->elements)[rFinal * colsM2 + cFinal] = ((float *)aux.elements)[0];
					}
				}
			}
			return 0;
		}
		else
		{
			// NO EXISTE NAMEVAR2.
			return -2;
		}
	}
	// NO EXISTE NAMEVAR1.
	return -1;
}
