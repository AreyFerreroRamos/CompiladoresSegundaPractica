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
 * Función que hace un printf del texto pasado con una variable como parametro.
 * typeFile identificara si pertenece a flex (0) o bison/funciones (1)
 **/
void debug(char *text, char *var, int typeFile);

/**
 * Función que hace un printf del texto pasado.
 * typeFile identificara si pertenece a flex (0) o bison/funciones (1)
 **/
void simpleDebug(char *text, int typeFile);

/**
 * Convierte un entero a string
 **/
char *iota(int num);

/**
 * Convierte un float a string
 **/
char *fota(float num);

/**
 * Reserva espacio en una variable char * para 200 carácteres como máximo y la devuleve
 **/
char *allocateSpaceForMessage();

/**
 * Dado un booleano (0/1) devuelve su contrario
 **/
int negateBoolean(int boolean);

/**
 * Dados los datos necesarios para crear una entrada en la symtab,
 * genera un objeto que contiene todos ellos y lo devuele.
 **/
value_info createValueInfo(char *value, char *type, char *lexema);

/**
 * Dados los parámetros necesarios para crear una estructura de tipo tensor_info (dim,
 * calcIndex y lexema) se crea dicha estructura.
 **/
tensor_info createTensorInfo(int dim, int calcIndex, char *lexema);

/**
 * Dado un string que contiene el id, posibles espacios y el símbolo "="
 * devuelve otro string con solo la parte del id
 ***/
char *getIdName(char *idWithAssign);

/**
 * Dada el nombre de una variable (de tipo Tensor) y una dimensión (posición en el vector
 * de dimensiones) devuelve cuantos elementos de la siguiente dimensión contiene dicha
 * dimensión (contenido de dicha posición en el vector de dimensiones).
 **/
int getDim(char *key, int num_dims);

/**
 * Dado un vector, reescribe sobre cada posición el cociente de esa misma posición por
 * la siguiente; excepto la última posición, que se guarda sin realizar ninguna operación.
 * A continuación se invierte el orden de los elementos del vector.
 **/
int *convert_invert_vector(int *vector, int dim);

/**
 * Dado un valor en formato caracter y el tipo de este valor lo convierte a
 * un void* interpretado como el tipo
 **/
void castValueToVoidPointer(void *ptr, char *value, char *type);

/**
 * Dadas dos listas  y el tipo de cada una las une en una sola lista que tendrá el tipo
 * prioritario (1.Float 2.Int)
 **/
void castTensorToVoidPointer(void *ptr, void *elements1, char *type1, int num_element1, void *elements2, char *type2, int num_element2);

/**
 * Dado un tipo devuelve el tamaño en bytes
 * */
int calculateSizeType(char *type);

/**
 * Dados dos nombres de tensores y una operación (suma o resta) realiza la operación y la almacena en un entrada de la symtab
 * con el nombre "tmp_for_tesor_result". Si algo va mal devolverá alguno de los mensajes de error.
 *  0 -> EJECUCIÓN CORRECTA, SE HAN CALCULADO LAS MATRICES Y LA ALMACENA EN SYMTAB
 * -1 -> ERROR, SE INTENTA SUMAR TENSOR CON VALOR
 * -2 -> NO SON TENSORES NINGUNO DE LOS DOS
 * -3 -> ERROR, ALGUN TENSOR NO ESTA DEFINIDO EN LA SYMTAB
 * -4 -> ERROR, LOS TENSORES SON DE DIFERENTE DIMENSION
 * -5 -> ERROR, NO SE HA PODIDO GUARDAR EL RESULTADO TEMPORAL
 */
int doTensorCalcs(char *nameVar1, char *nameVar2, char *operation);

/**
 * Dados dos nombres de tensores y una operación (solo se admite producto) realiza la operación y la almacena en un entrada de la symtab
 * con el nombre "tmp_for_tesor_result". Si algo va mal devolverá alguno de los mensajes de error.
 *
 */
int doTensorProduct(char *nameVar1, char *nameVar2, char *operation);

/**
 * Comprueba si las dimensiones de los tensores son compatibles para poder hacer el prodcto.
 * Si algo va mal devolverá alguno de los mensajes de error.
 * 0 -> EJECUCIÓN CORRECTA, TENSORES MULTIPLICABLES
 * -1 -> ERROR, ESTOS TENSORES NO SE PUEDEN MULTIPLICAR
 * -2 -> ERROR, NO ESTA IMPLEMENTADO EL PRODUCTO DE TENSORES DE MÁS DE 2 DIMENSIONES
 */
int isPossibleTensorProduct(int *elemDims1, int numDims1, int *elemDims2, int numDims2);

/**
 *
 */
int doNumberProductTensor(char *number, char type, char *nameTensor);

/**
 *
 */
int doTensorProductTensor(sym_value_type matrix1, sym_value_type matrix2, sym_value_type tmp);

int asignacionTensor(sym_value_type *result, int posicion, value_info v1, value_info v2, char *op)

/**
* Dados dos numeros devuelve 1 si el primero es más grande, 0 si son iguales o -1 si es más pequeño
*/
int maxNum(float a, float b);

/**
 * Dado un tensor, el nombre de la variable i si se quiere imprimir en el archivo de salida o en consola
 * imprime los valores almacenados dentro del tensor de forma continua (como si fuese un vector de una dimensión)
 */
void printTensor(char *nameVar, sym_value_type tensor, int inFile);

// FUNCIONES DE CONTROL DE ERRORES

/**
 * Dados dos tipos comprueba que son iguales y en ese caso devuelve 1,
 * en caso contrario 0.
 **/
int isSameType(char *type1, char *type2);

/**
 * Dado un tipo controla si es entero o float y devuelve 1 en caso de serlo
 * o 0 si no lo es
 **/
int isNumberType(char *type);

// FUNCIONES PARA REALIZAR OPERACIONES

/**
 * Dados dos operadores y un operando realiza la operacion y la almacena
 * en finish_val
 **/
int doAritmeticOperation(value_info v1, char *operand, value_info v2, value_info *finish_val);

/**
 * Dados dos numeros y un operando relacional devuelve el resultado de la expresión
 * "num1 op num2".
 **/
int doRelationalOperation(float num1, char *op, float num2);

/**
 * Dados dos enteros y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2" dentro de res. Además si la operación no es válida devuelve 0, o 1 en
 * caso de que si sea correcta.
 **/
int intOperations(int num1, int num2, char *op, int *res);

/**
 * Dados dos floats y un operando aritmético, devuelve el resultado de la expresión
 * "num1 op num2 "dentro de res. Además si la operación no es válida devuelve 0, o 1 en
 * caso de que si sea correcta.
 **/
int floatOperations(float num1, float num2, char *op, float *res);

/**
 * Dado un string comprueba si es una key de la tabla de simbolos.
 * Si es una key devuelve el size guardado en la variable, sino devuelve
 * el strlen() del string pasado como parámetro. En el caso que al buscar
 * en la tabla de símbolos de un error distinto a no encontrado se devolverá -1.
 **/
int lenght(char *key);

#endif
