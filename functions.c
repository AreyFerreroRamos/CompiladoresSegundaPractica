#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "functions.h"

extern int num_tmp_variable;
extern int sq;	        // Siguiente linia que toca escribir
extern int lengthCode; // Nombre de linias de c3a
extern char **c3a;	   // Conjunto de lineas que formarán el codigo de 3 adreces

char *generateTmpId()
{
	char *id;
	id = (char *) malloc(sizeof(char) * ID_MAX_LENGTH);
	sprintf(id, "%s%i", TMP_BASE_ID, num_tmp_variable);
	num_tmp_variable++;
	return id;
}

void emet(char *type, int nArgs, ...)
{
    va_list ap;
    va_start(ap, nArgs);

    char *instruction;
    if (isSameType(type, INSTR_COPY))
    {
        value_info *data = malloc(sizeof(value_info)*2);
        for (int i = 0; i < nArgs; i++)
        {
            data[i] = va_arg(ap, value_info);
        }
        controlTensorIndex(&data[0]);
        controlTensorIndex(&data[1]);
        instruction = emetCopy(data[0], data[1]);
    }
    else if (isSameType(type, INSTR_ADDI) || isSameType(type, INSTR_ADDD)
             || isSameType(type, INSTR_SUBI) || isSameType(type, INSTR_SUBD)
             || isSameType(type, INSTR_MULI) || isSameType(type, INSTR_MULD)
             || isSameType(type, INSTR_DIVI) || isSameType(type, INSTR_DIVD)
             || isSameType(type, INSTR_MODI))
    {
        value_info *data = malloc(sizeof(value_info)*3);
        for (int i = 0; i < 3; i++)
        {
            data[i] = va_arg(ap, value_info);
        }
        instruction = emetOperation(type, data[0], data[1], data[2]);
    }
    else if(isSameType(type,INSTR_START))
    {
        char *func = strdup(va_arg(ap,char*));
        instruction = emetStart(func);
    }
    else if(isSameType(type,INSTR_END))
    {
        int isAction = va_arg(ap,int);
        //Si no tiene valor de retorno y la última línea no es un RETURN
        if( isAction==1 && !isSameType(c3a[sq-1],INSTR_RETURN))
        {
            writeLine(sq,INSTR_RETURN);
        }
        writeLine(sq,INSTR_END);
        instruction = "";
    }
    else if(isSameType(type,INSTR_RETURN))
    {
        if(nArgs==0){
            instruction = emetReturn(NULL);
        }else{
            char *value = strdup(va_arg(ap,char*));
            instruction = emetReturn(value);
        }
    }

    va_end(ap);
    writeLine(sq, instruction);
}

void controlTensorIndex(value_info *v){
    if(isSameType(v->valueInfoType,TENS_T)){
        if(isSameType(v->index.valueInfoType,VAR_T)){
            char *tmp;
            value_info v1,v2,v3;

            //MULTIPLICAMOS POR ESPACIO DEL TIPO
            tmp=generateTmpId();
            v1 = createValueInfo(tmp,INT32_T,VAR_T,generateEmptyValueInfoBase());
            v2 = createValueInfo(v->index.value,INT32_T,VAR_T,generateEmptyValueInfoBase());
            v3 = createValueInfo(itos(calculateSizeType(v2.type)),INT32_T,LIT_T,generateEmptyValueInfoBase());
            classifyOperation(OP_ARIT_MULT,v1,v2,v3);
            v->index= createValueInfoBase(v1.value,v1.type,v1.valueInfoType);
        }else{
            v->index.value = itos((atoi(v->index.value ))* calculateSizeType(v->index.type));
        }
    }
}

void emetTensor(char *lexema, tensor_ini_info tensor)
{
    value_info v1, v2;
    for (int i = 0; i < tensor.num_elem; i++)
    {
        v1 = createValueInfo(lexema, tensor.type, TENS_T, createValueInfoBase(itos(i), INT32_T, LIT_T));
        v2 = createValueInfo(tensor.elements[i].value, tensor.elements[i].type, tensor.elements[i].valueInfoType,generateEmptyValueInfoBase());
        emet(INSTR_COPY,2, v1, v2);
    }
}

void classifyOperation(char *operation, value_info v1, value_info v2, value_info v3)
{
    if (isSameType(v1.type, INT32_T))
    {
        if (isSameType(operation, OP_ARIT_SUMA))
        {
            emet(INSTR_ADDI,3, v1, v2, v3);
        }
        else if (isSameType(operation, OP_ARIT_RESTA))
        {
            emet(INSTR_SUBI,3, v1, v2, v3);
        }
        else if (isSameType(operation, OP_ARIT_MULT))
        {
            emet(INSTR_MULI,3, v1, v2, v3);
        }
        else if (isSameType(operation, OP_ARIT_DIV))
        {
            emet(INSTR_DIVI,3, v1, v2, v3);
        }
        else if (isSameType(operation, OP_ARIT_MOD))
        {
            emet(INSTR_MODI,3, v1, v2, v3);
        }
    }
    else
    {
        if (isSameType(operation, OP_ARIT_SUMA))
        {
            emet(INSTR_ADDD,3, v1, v2, v3);
        }
        else if (isSameType(operation, OP_ARIT_RESTA))
        {
            emet(INSTR_SUBD,3, v1, v2, v3);
        }
        else if (isSameType(operation, OP_ARIT_MULT))
        {
            emet(INSTR_MULD,3, v1, v2, v3);
        }
        else if (isSameType(operation, OP_ARIT_DIV))
        {
            emet(INSTR_DIVD,3, v1, v2, v3);
        }
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
			yyerror("No es posible realizar la operación módulo entre dos números reales.");
		}
	}
	finish_val->valueInfoType = LIT_T;
	finish_val->index = generateEmptyValueInfoBase();
}


value_info_base *castValueInfoToValueInfoBase(value_info v)
{
    value_info_base *aux = malloc(sizeof(value_info_base));
    aux->value = strdup(v.value);
    aux->type = strdup(v.type);
    aux->valueInfoType = strdup(v.valueInfoType);
    return aux;
}

value_info_base *joinElementsVectors(value_info_base * vec1, value_info_base * vec2, int numElemVec1, int numElemVec2)
{
    int totalElem = numElemVec1 + numElemVec2;
    value_info_base *aux = realloc(vec1,totalElem * sizeof(value_info_base));
    int cont = numElemVec1;
    for (int i = 0; i < numElemVec2; i++)
    {
        aux[cont++] = vec2[i];
    }
    return aux;
}

void invertVector(int *vector, int dim)
{
    int aux;
    for (int i = 0; i < dim / 2; i++)
    {   // Invertir el vector.
        aux = vector[i];
        vector[i] = vector[dim - i - 1];
        vector[dim - i - 1] = aux;
    }
}

int getDim(char *key, int index_dim)
{
    sym_value_type entry = getEntry(key);
    if (index_dim >= entry.num_dim)
    {
        yyerror("Estás accediendo a una dimensión no válida.");
    }
    return entry.elem_dims[index_dim];
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

void addValueInfoBase(value_info_base *list,int numElem,value_info_base toAdd)
{
    if(numElem==0){
        list = malloc(sizeof(value_info_base));
    }else{
        list = realloc(list, sizeof(value_info_base)*(numElem+1));
    }
    list[numElem]=toAdd;
}

sym_value_type castValueInfoBaseToSymValueType(value_info_base v){
    return createSymValueType(v.type,0,0,NULL,NULL,v.valueInfoType);
}

char *calculateNewIndex(int dim, value_info_base calcIndex,value_info index){

    printf("VALUE:%s TYPE1:%s TYPE2:%s\n",calcIndex.value,calcIndex.type,calcIndex.valueInfoType);
    char *nameTmp1 = generateTmpId();
    value_info tmp1 = createValueInfo(nameTmp1, INT32_T, VAR_T, generateEmptyValueInfoBase());
    value_info one = createValueInfo("1", INT32_T, LIT_T, generateEmptyValueInfoBase());
    value_info calcIndexOld = createValueInfo(calcIndex.value, calcIndex.type, calcIndex.valueInfoType, generateEmptyValueInfoBase());
    value_info dimension = createValueInfo(itos(dim), INT32_T, LIT_T, generateEmptyValueInfoBase());
    emet(INSTR_MULI,3, tmp1, calcIndexOld, dimension);
    if(isSameType(index.valueInfoType,VAR_T))
    {
        char *nameTmp2 = generateTmpId();
        value_info tmp2 = createValueInfo(nameTmp2, INT32_T, VAR_T, generateEmptyValueInfoBase());
        emet(INSTR_SUBI,3, tmp2, index,one);
        emet(INSTR_ADDI,3, tmp1, tmp1, tmp2);
    }
    else
    {
        index.value = itos(atoi(index.value)-1);
        emet(INSTR_ADDI,3, tmp1, tmp1, index);
    }
    return nameTmp1;
}