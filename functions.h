#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "tipus.h"
#include "utils.h"
#include "support.h"

/* FUNCIONES QUE SE USAN SOLAMENTE EN FLEX Y BISON */

/**
 * Genera un nuevo Id de forma incremental con la forma $tXXXXXXXX (donde las X son números)
 * para una variable temporal dentro del c3a
 **/
char *generateTmpId();

/**
 * Dado un operador y los operadores ejecuta la función emet() con la
 * instrucción adecuada
 */
void classifyOperation(char *operation, value_info v1, value_info v2, value_info v3);

/**
 *  Dado un tipo de instrucción y los posibles valores dentro de ella, se encarga de gestionar
 *  como se escribe la instruccion o conjunto de intrucciones y añadirla al c3a
 **/
void emet(char *type, value_info v1, value_info v2, value_info v3);

/**
 *  Dada una linea y un string, se encarga de añadir de añadir el string dentro del c3a
 */
void writeLine(int line, char *instruction);

/**
 * Given two numeric type returns the new one that result to
 * operate them
 */
char *getNewType(char *type1, char *type2);

/**
 * Dado un tipo devuelve el tamaño en bytes.
 **/
int calculateSizeType(char *type);

/**
 * Dados dos operadores y un operando realiza la operacion y la almacena
 * en finish_val.
 **/
void doAritmeticOperation(value_info v1, char *operand, value_info v2, value_info *finish_val);

#endif