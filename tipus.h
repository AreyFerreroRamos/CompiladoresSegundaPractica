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

// TIPOS DE INSTRUCCIÓN.

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
#define INSTR_COPY "COPY"       // Se asigna el contenido de una variable a otra variable.
#define INSTR_CONSULT "CONSULT" // Se asigna el contenido de una posición de un tensor a una variable.
#define INSTR_ASSIGN "ASSIGN"   // Se asigna el contenido de una variable en una posición del tensor.
#define INSTR_PARAM "PARAM"
#define INSTR_CALL "CALL"
#define INSTR_PUT "PUT"
#define INSTR_HALT "HALT"
#define INSTR_RETURN "RETURN"

/**
 * Esta estructura contiene los campos necesarios para gestionar un elemento (literal o una variable)
 * en un punto del programa en que dicho elemento se utiliza como índice para acceder a un tensor.
 * Los tensores y las funciones también pueden comportarse como índices de otro tensor,
 * pero serán convertidos en variables temporales durante el proceso de generación del código intermedio (tiempo de compilación).
 * Esta estructura contiene los mismos campos que la estructura value_info (definida justo después) con excepción del índice.
 * Si las estructuras de tipo 'typedef struct' permitiesen acceso recursivo, este registro no sería necesario.
 */
 typedef struct
{
    char *type;             // Tipo del elemenento.
    char *value;            // Valor del elemento en caso de tratarse de un literal o lexema en caso de tratarse de una variable.
    char *valueInfoType;    // Indica si el elemento es una variable o un literal.
} value_info_base;

 /**
  * Esta estructura contiene los campos necesarios para gestionar un elemento (literal, variable, tensor o función)
  * en cualquier punto del programa en el que sea usado, salvo cuando se utiliza como índice para acceder a un tensor.
  */
typedef struct
{
	char *type;                 // Tipo del elemenento.
	char *value;                // Valor del elemento en caso de tratarse de un literal o lexema en caso de tratarse de una variable, un tensor o una función.
	char *valueInfoType;        // Indica si el elemento es un literal, una variable, un tensor o una función.
	value_info_base index;      // Índice de acceso en caso de ser un tensor. Si el elemento no es un tensor, este campo será nulo.
} value_info;

/**
 * Esta estructura contiene los campos necesarios para gestionar un tensor
 * en el punto del programa en el que se accede a una posición del mismo.
 */
typedef struct
{
	int index_dim;              // Posición actual del vector de dimensiones del tensor.
	value_info_base calcIndex;  // Variable sobre la que se van acumulando los cálculos parciales para acceder a la posición del tensor tratándolo como si fuera un vector.
	char *lexema;               // Nombre de la variable tensor.
} tensor_info;

/**
 * Esta estructura contiene los campos necesarios para gestionar un tensor elemento a elemento
 * en el punto del programa en el que este es definido.
 */
typedef struct
{
	int dim;                    // Dimension concreta que se esta evaluando actualmente.
	char *type;                 // Tipo concreto que se esta evaluando actualmente.
	value_info_base *elements;  // Valores dentro del componente.
	int num_elem;               // Número de elementos del tensor.
} tensor_ini_info;

#endif
