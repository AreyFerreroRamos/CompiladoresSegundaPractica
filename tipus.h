#ifndef TIPUS_H

#define TIPUS_H

#define MESSAGE_MAX_LENGTH 200
#define INT_MAX_LENGTH_STR 10
#define FLOAT_MAX_LENGTH_STR 12
#define ID_MAX_LENGTH 10
#define INSTR_MAX_LENGTH 200

#define TMP_BASE_ID "$t"
#define INT32_T "Int32"
#define FLOAT64_T "Float64"
#define BOOLEAN_T "Bool"
#define STRING_T "String"
#define TENSOR_T "Tensor"

#define FUNC_T "_FUNC_T"
#define VAR_T "_VAR_T"
#define TENS_T "_TENS_T"
#define LIT_T "_LIT_T"

#define OP_ARIT_SUMA "+"
#define OP_ARIT_RESTA "-"
#define OP_ARIT_MULT "*"
#define OP_ARIT_DIV "/"
#define OP_ARIT_MOD "%"
#define OP_ARIT_POTENCIA "^"

#define OP_REL_HIGH ">"
#define OP_REL_LESS "<"
#define OP_REL_HE ">="
#define OP_REL_LE "<="
#define OP_REL_EQUAL "=="
#define OP_REL_DIFF "!="

#define OP_BOOL_AND "&&"
#define OP_BOOL_OR "||"

/* TIPOS DE INSTRUCCIÓN */

#define INSTR_START "START"
#define INSTR_END "END"
#define INSTR_ADDI "ADDI"
#define INSTR_ADDD "ADDD"
#define INSTR_SUBI "SUBI"
#define INSTR_SUBD "SUBD"
#define INSTR_MULI "MULI"
#define INSTR_MULD "MULD"
#define INSTR_DIVI "DIVI"
#define INSTR_DIVD "DIVD"
#define INSTR_MODI "MODI"
#define INSTR_I2D "I2D"
#define INSTR_D2I "D2I"
#define INSTR_COPY "COPY"                       // Se asigna el contenido de una variable a otra variable.
#define INSTR_COPY_TO_TENSOR "COPY2TENSOR"      // Instrucción t[] = var.
#define INSTR_COPY_FROM_TENSOR "COPYFROMTENSOR" // Instrucción var = t[].
#define INSTR_CONSULT "CONSULT"                 // Se asigna el contenido de una posición de un tensor a una variable.
#define INSTR_ASSIGN "ASSIGN"                   // Se asigna el contenido de una variable en una posición del tensor.
#define INSTR_PARAM "PARAM"
#define INSTR_CALL "CALL"
#define INSTR_PUT "PUT"
#define INSTR_HALT "HALT"
#define INSTR_RETURN "RETURN"

/**
 * Esta estructura contiene los campos necesarios para gestionar un ID.
 */
typedef struct
{
    char *lexema;   // Nombre del identificador.
    int length;     // Tamaño en bytes del identificador.
    int line;       // Línea en la que se encuentra el identificador.
} ident_info;

/**
 * Esta estructura contiene los campos necesarios para gestionar un elemento (literal, variable, tensor o función)
 * en cualquier punto del programa en el que sea usado.
 */
typedef struct
{
	char *type;                 // Tipo del elemenento.
	char *value;                // Valor del elemento en caso de tratarse de un literal o lexema en caso de tratarse de una variable, un tensor o una función.
	char *valueInfoType;        // Indica si el elemento es un literal, una variable, un tensor o una función.
} value_info;

/**
 * Esta estructura contiene los campos necesarios para gestionar un tensor
 * en el punto del programa en el que se accede a una posición del mismo.
 */
typedef struct
{
	int index_dim;              // Posición actual del vector de dimensiones del tensor.
    value_info calcIndex;       // Variable sobre la que se van acumulando los cálculos parciales para acceder a la posición del tensor al tratarlo como si fuera un vector.
	char *lexema;               // Nombre de la variable tensor.
} tensor_info;

/**
 * Esta estructura contiene los campos necesarios para gestionar una lista de elementos
 * de tipo value_info.
 */
typedef struct
{
    value_info *elements;    // Lista de variables o argumentos.
    int numElem;             // Número de variables o argumentos.
} elements_list;

/**
 * Esta estructura contiene los campos necesarios para gestionar un tensor elemento a elemento
 * en el punto del programa en el que este es definido.
 */
typedef struct
{
	int dim;                    // Dimension concreta que se esta evaluando actualmente.
	char *type;                 // Tipo concreto que se esta evaluando actualmente.
    elements_list elemList;     // Lista con los elementos del tensor.
} tensor_ini_info;

/**
 * Esta estructura contiene los campos necesarios para gestionar las funciones y sus cabeceras.
 */
typedef struct
{
    char *funcName;         // Nombre de la función.
    char *returnType;       // Parametro de retorno (puede ser nulo).
    elements_list elemList; // Lista con los parámetros de la función.
} func_param_info;

#endif