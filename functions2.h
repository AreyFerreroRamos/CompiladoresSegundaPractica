#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "tipus.h"
#include "symtab.h"

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

int init_analisi_lexic(char *);
int init_analisi_sintactic(char *);
int analisi_semantic();
int end_analisi_lexic();
int end_analisi_sintactic();
void yyerror(char *);

// FUNCIONES DE UTILIDAD

/**
 * Genera un nuevo Id de forma incremental con la forma $tXXXXXXXX (donde las X son números)
 * para una variable temporal dentro del c3a
 **/
char *generateTmpId();

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
 *  Escribe el c3a por consola y en el fichero de salida
 */
void printCode3Adresses();

/**
 * Dado un tipo controla si es entero o float y devuelve 1 en caso de serlo
 * o 0 si no lo es.
 **/
int isNumberType(char *type);

/**
 * Dados dos tipos comprueba que son iguales y en ese caso devuelve 1,
 * en caso contrario 0.
 **/
int isSameType(char *type1, char *type2);

/**
 * Convierte un entero a string.
 **/
char *itos(int num);

/**
 * Convierte un float a string.
 **/
char *ftos(float num);

/**
 * Reserva espacio en una variable char * para 200 carácteres como máximo y la devuleve.
 **/
char *allocateSpaceForMessage();

/**
 * Función que hace un printf del texto pasado con una variable como parametro.
 * typeFile identificara si pertenece a flex (0) o bison/funciones (1).
 **/
void debug(char *text, char *var, int typeFile);

/**
 * Función que hace un printf del texto pasado.
 * typeFile identificara si pertenece a flex (0) o bison/funciones (1).
 **/
void simpleDebug(char *text, int typeFile);

/**
 * Dados los datos necesarios para crear una entrada en la symtab,
 * genera un objeto que contiene todos ellos y lo devuele.
 **/
sym_value_type createSymValueType(char *type, int size, int numDim, int *elemDims, void *elements, char *entryType);

/**
 * Dados los datos necesarios para crear una unidad de datos para bison,
 * genera un objeto que contiene todos ellos y lo devuele.
 **/
value_info createValueInfo(char *value, char *type, char *lexema, char *valueInfoType, int index);

/**
 * Dado un tipo devuelve el tamaño en bytes.
 **/
int calculateSizeType(char *type);

/**
 * Dado un texto con "%s donde quiera poner un argumento, el numero de argumentos, y los argumentos
 * devuelve un string con los argumentos dentro del mensaje
 */
char *generateString(char *message, int nArgs, ...);

#endif