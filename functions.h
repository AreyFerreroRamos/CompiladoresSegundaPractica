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
 * Dada una variable de tipo tensor y la estructura necesaria para gestionarlo durante su definición,
 * se generará el c3a necesario para imprimir, línea a línea, la inicialización de cada posición del vector,
 * Para lograrlo, se llama a la función anterior para cada posición del tensor.
 */
void emetTensor(char *lexema, tensor_ini_info tensor);

/**
 *  Dada una linea y un string, se encarga de añadir de añadir el string dentro del c3a
 */
void writeLine(int line, char *instruction);

/**
 * Dados dos tipos númericos, se devuelve el tipo que resulta de operar ambos números.
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

/**
 * Dado un value_info que llegará como valor de un tensor devuelve un string de una posición
 * con los datos pasados
 */
value_info_base *castValueInfoToValueInfoBase(value_info v);

/**
 * Dados dos arrays de value_info_base devuelve un nuevo array con la union de los dos anteriores
 */
value_info_base *joinElementsVectors(value_info_base * vec1,value_info_base * vec2,int numElemVec1,int numElemVec2);

/**
 * Dado un vector, se invierte el orden de los elementos que lo componen.
 **/
void invertVector(int *vector, int dim);

/**
 * Dado el nombre de una variable de tipo tensor y la posición de una dimensión en el vector de dimensiones del tensor,
 * se comprueba si el índice es válido y, en caso afirmativo, se recupera dicha dimensión.
 */
int getDim(char *key, int index_dim);

/**
 * Dado un vector de enteros que representa las dimensiones de un tensor, devuelve el número
 * de elemntos que hay dentro de el
 **/
int getAcumElemDim(int *elem_dim, int num_dim);

#endif