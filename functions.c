#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

extern int num_tmp_variable;
extern int sq;		   // Siguiente linia que toca escribir
extern int lengthCode; // Nombre de linias de c3a
extern char **c3a;	   // Conjunto de lineas que formarán el codigo de 3 adreces

char *generateTmpId()
{
	char *id;
	id = (char *)malloc(sizeof(char) * ID_MAX_LENGTH);
	sprintf(id, "%s%i", TMP_BASE_ID, num_tmp_variable);
	num_tmp_variable++;
	return id;
}

void classifyOperation(char *operation, value_info v1, value_info v2, value_info v3)
{
	if (isSameType(operation, OP_ARIT_SUMA))
	{
		if (isSameType(v1.type, INT32_T))
		{
			emet(INSTR_ADDI, v1, v2, v3);
		}
		else
		{
			emet(INSTR_ADDD, v1, v2, v3);
		}
	}
	else if (isSameType(operation, OP_ARIT_RESTA))
	{
		if (isSameType(v1.type, INT32_T))
		{
			emet(INSTR_SUBI, v1, v2, v3);
		}
		else
		{
			emet(INSTR_SUBD, v1, v2, v3);
		}
	}
	else if (isSameType(operation, OP_ARIT_MULT))
	{
		if (isSameType(v1.type, INT32_T))
		{
			emet(INSTR_MULI, v1, v2, v3);
		}
		else
		{
			emet(INSTR_MULD, v1, v2, v3);
		}
	}
	else if (isSameType(operation, OP_ARIT_DIV))
	{
		if (isSameType(v1.type, INT32_T))
		{
			emet(INSTR_DIVI, v1, v2, v3);
		}
		else
		{
			emet(INSTR_DIVD, v1, v2, v3);
		}
	}
	else if (isSameType(operation, OP_ARIT_MOD))
	{
		emet(INSTR_MODI, v1, v2, v3);
	}
	else if (isSameType(operation, OP_ARIT_POTENCIA))
	{
	}
}

void emet(char *type, value_info v1, value_info v2, value_info v3)
{
	char *instruction;
	if (isSameType(type, INSTR_COPY))
	{
		instruction = emetCopy(v1, v2);
		writeLine(sq, instruction);
	}
	else if (isSameType(type, INSTR_ADDI) || isSameType(type, INSTR_ADDD) 
	|| isSameType(type, INSTR_SUBI) || isSameType(type, INSTR_SUBD) 
	|| isSameType(type, INSTR_MULI) || isSameType(type, INSTR_MULD) 
	|| isSameType(type, INSTR_DIVI) || isSameType(type, INSTR_DIVD) 
	|| isSameType(type, INSTR_MODI))
	{
		instruction = emetOperation(type, v1, v2, v3);
		writeLine(sq, instruction);
	}
}

void writeLine(int line, char *instruction)
{
	if (c3a == NULL)
	{
		c3a = malloc(sizeof(char *));
		lengthCode++;
		sq++;
	}
	else if (line >= lengthCode)
	{
		c3a = realloc(c3a, (lengthCode + 1) * sizeof(char *));
		lengthCode++;
		sq++;
	}
	c3a[line] = strdup(instruction);
}

char *getNewType(char *type1, char *type2)
{
	if (isSameType(type1, INT32_T) && isSameType(type2, INT32_T))
	{
		return INT32_T;
	}
	else
	{
		return FLOAT64_T;
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

void doAritmeticOperation(value_info v1, char *operand, value_info v2, value_info *finish_val)
{

	if (strcmp(v1.type, "Int32") == 0 && strcmp(v2.type, "Int32") == 0)
	{
		finish_val->type = "Int32";
		int resInt;
		intOperations(atoi(v1.value), atoi(v2.value), operand, &resInt);
		finish_val->value = itos(resInt);
	}
	else
	{
		if (strcmp(operand, "%") != 0)
		{
			finish_val->type = "Float64";
			float resFloat;
			floatOperations(atof(v1.value), atof(v2.value), operand, &resFloat);
			finish_val->value = ftos(resFloat);
		}
		else
		{
			yyerror("No se puede hacer módulo con floats");
		}
	}
	finish_val->valueInfoType = LIT_T;
	finish_val->index = NULL;
}


value_info_base *castValueInfoToTensorIniInfo(value_info v){
    value_info_base *aux = malloc(sizeof(value_info_base));
    aux->valueInfoType = _strdup(v.valueInfoType);
    aux->type = _strdup(v.type);
    aux->value = _strdup(v.value);
    return aux;
}

value_info_base *joinElementsVectors(value_info_base * vec1,value_info_base * vec2,int numElemVec1,int numElemVec2){
    int totalElem = numElemVec1+numElemVec2;
    value_info_base *aux = realloc(vec1,totalElem*sizeof(value_info_base));
    int cont = numElemVec1;
    for(int i = 0;i<numElemVec2;i++){
        aux[cont++] = vec2[i];
    }
    return aux;
}

void invertVector(int *vector, int dim)
{

    int aux;
    int i;

    for (i = 0; i < dim / 2; i++)
    { // Invertir el vector.
        aux = vector[i];
        vector[i] = vector[dim - i - 1];
        vector[dim - i - 1] = aux;
    }
}