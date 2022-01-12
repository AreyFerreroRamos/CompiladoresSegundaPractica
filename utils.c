#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"

extern int yylineno;

void yyerror(char *explanation)
{
    fprintf(stderr, "Error: %s, in line %d\n", explanation, yylineno);
    exit(EXIT_FAILURE);
}

char *generateString(char *message, int nArgs, ...)
{
    va_list ap;
    va_start(ap, nArgs);
    char **params = malloc(sizeof(char *) * nArgs);
    for (int i = 0; i < nArgs; i++)
    {
        params[i] = va_arg(ap, char *);
    }
    va_end(ap);

    char *string = allocateSpaceForMessage();
    switch (nArgs)
    {
    case 0:
        sprintf(string, message);
        break;
    case 1:
        sprintf(string, message, params[0]);
        break;
    case 2:
        sprintf(string, message, params[0], params[1]);
        break;
    case 3:
        sprintf(string, message, params[0], params[1], params[2]);
        break;
    case 4:
        sprintf(string, message, params[0], params[1], params[2], params[3]);
        break;
    case 5:
        sprintf(string, message, params[0], params[1], params[2], params[3], params[4]);
        break;
    case 6:
        sprintf(string, message, params[0], params[1], params[2], params[3], params[4], params[5]);
        break;
    case 7:
        sprintf(string, message, params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
        break;

    default:
        yyerror("Estas usando mal la función generateString(), nArgs debe estar entre 0 y 7");
    }
    return strdup(string);
}

value_info createValueInfo(char *value, char *type, char *lexema, char *valueInfoType, char *index)
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
    aux.valueInfoType = strdup(valueInfoType);
    aux.index = index;
    return aux;
}

sym_value_type createSymValueType(char *type, int size, int numDim, int *elemDims, void *elements, char *entryType)
{
    sym_value_type aux;
    aux.type = strdup(type);
    aux.entryType = strdup(entryType);
    aux.size = size;
    aux.num_dim = numDim;
    aux.elem_dims = elemDims;
    aux.elements = elements;
    return aux;
}

int isNumberType(char *type)
{
    return (strcmp(type, INT32_T) == 0 || strcmp(type, FLOAT64_T) == 0);
}

int isSameType(char *type1, char *type2)
{
    return (strcmp(type1, type2) == 0);
}

char *itos(int num)
{
    char *string = (char *)malloc(sizeof(char) * INT_MAX_LENGTH_STR);
    sprintf(string, "%i", num);
    return string;
}

char *ftos(float num)
{
    char *string = (char *)malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
    sprintf(string, "%.2f", num);
    return string;
}

char *allocateSpaceForMessage()
{
    char *message;
    message = (char *)malloc(sizeof(char) * MESSAGE_MAX_LENGTH);
    return message;
}

void debug(char *text, char *var, int typeFile)
{
    // flex
    if (typeFile == 0)
    {
        // printf(text, var);
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

sym_value_type getEntry(char* key){
    sym_value_type entry;
    int response = sym_lookup(key, &entry);
    if (response == SYMTAB_OK){return entry;}
    else if(response == SYMTAB_NOT_FOUND){yyerror(generateString("No se ha encontrado la clave %s en la symtab",1,key));}
    else{yyerror("Algun problema buscando el valor en la symtab");}
}


void addOrUpdateEntry(char* key, sym_value_type entry){
    int response = sym_enter(key, &entry);
    if(response == SYMTAB_NOT_FOUND){yyerror(generateString("No se ha encontrado la clave %s en la symtab",1,key));}
    else if(response == SYMTAB_NO_MEMORY){yyerror("No hay más memoria");}
    else if(response != SYMTAB_OK && response != SYMTAB_DUPLICATE ){yyerror("Algun problema guardando el valor en la symtab");}
}

