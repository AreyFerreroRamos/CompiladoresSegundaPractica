#ifndef UTILS_H
#define UTILS_H
#include "tipus.h"
#include "symtab.h"

/* FUNCIONES QUE SE USAN EN MÁS DE UN ÁMBITO, MAS DE UN SITIO ENTRE FLEX, BISON, FUNCTIONS Y SUPPORT */

/**
 * Muestra un mensaje de error y finaliza la ejecución de la compilación
 */
void yyerror(char *);

/**
 * Dado un texto con "%s donde quiera poner un argumento, el numero de argumentos, y los argumentos
 * devuelve un string con los argumentos dentro del mensaje
 */
char *generateString(char *message, int nArgs, ...);

/**
 * Dados los datos necesarios para crear una unidad de datos para bison,
 * genera un objeto que contiene todos ellos y lo devuele.
 **/
value_info createValueInfo(char *value, char *type, char *lexema, char *valueInfoType, char *index);

/**
 * Dados los datos necesarios para crear una entrada en la symtab,
 * genera un objeto que contiene todos ellos y lo devuele.
 **/
sym_value_type createSymValueType(char *type, int size, int numDim, int *elemDims, void *elements, char *entryType);

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
* La función busca en la symtab la variable y la devuelve. Si se produce algún error
* se lanzará un yyerror()
**/
sym_value_type getEntry(char* key);

/**
* La función añade o actualiza (si ya existía) una entrada en la symtab. Si se produce algún error
* se lanzará un yyerror()
**/
void addOrUpdateEntry(char* key, sym_value_type entry);

#endif