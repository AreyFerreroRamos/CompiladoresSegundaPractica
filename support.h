#ifndef SUPPORT_H

#define SUPPORT_H

#include <math.h>
#include "tipus.h"
#include "utils.h"

/* FUNCIONES QUE SOLAMENTE SON LLAMADAS DESDE EL FICHERO FUNCTIONS.C O DESDE EL MAIN */

//-------------------------EMET FUNCTIONS----------------------------

/**
 * Crea una instrucción de tipo copy con los parametros pasados
 * y la devuelve
 */
char *emetCopy(value_info v1, value_info v2);

/**
 * Crea una instrucción de asignación con una operación aritmetica
 * y la devuelve
 */
char *emetOperation(char *op, value_info v1, value_info v2, value_info v3);

/**
 * Dado un valor o el lexema de una variable temporal,
 * devuelve al instrucción "PUT [valor | lexema].
 */
char *emetPut(char *element);

/**
 * Dado el nombre la función devuelve la instrucción "START [nombre función]"
 */
char *emetStart(char *func);

/**
 * Dado el valor a devolver en un RETURN  devuelve la instrucción "RETURN [valor]".
 * Si el valor es NULL se devolvera solo "RETURN"
 */
char *emetReturn(char *var);

/**
 * Dada la información de la variable a la que se le asigna algun valor
 * devuelve su correspondencia a c3a
 */
char *addV1(value_info v1);

/**
 * Dada la información del primer operando de una instruccion c3a y el inicio
 * de la instrucción, devuelve su correspondencia a c3a
 */
char *addV2(char *instruction, value_info v2);

/**
 * Dada una operación y el inicio de la instrucción la añade a la instrucción
 */
char *addOperation(char *instruction, char *op);

/**
 * Dada la información del segundo operando de una instruccion c3a y el inicio
 * de la instrucción devuelve su correspondencia a c3a
 */
char *addV3(char *instruction, value_info v3);

//-------------------------------------------------------------------

/**
 *  Escribe el c3a por consola y en el fichero de salida
 */
void printCode3Adresses();

/**
 * Dados dos enteros y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2" dentro de res. Además si la operación no es válida devuelve 0, o 1 en
 * caso de que si sea correcta.
 **/
void intOperations(int num1, int num2, char *op, int *res);

/**
 * Dados dos floats y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2 "dentro de res. Además si la operación no es válida devuelve 0, o 1 en
 * caso de que si sea correcta.
 **/
void floatOperations(float num1, float num2, char *op, float *res);

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

int init_analisi_lexic(char *);
int init_analisi_sintactic(char *);
int analisi_semantic();
int end_analisi_lexic();
int end_analisi_sintactic();

#endif