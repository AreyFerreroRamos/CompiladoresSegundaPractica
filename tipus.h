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
#define BOOLEAN_T "Boolean"
#define STRING_T "String"

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

#define INSTR_START "_I_START"
#define INSTR_END "_I_END"
#define INSTR_ADDI "ADDI"
#define INSTR_ADDD "ADDD"
#define INSTR_SUBI "SUBI"
#define INSTR_SUBD "SUBD"
#define INSTR_MULI "MULI"
#define INSTR_MULD "MULD"
#define INSTR_DIVI "DIVI"
#define INSTR_DIVD "DIVD"
#define INSTR_MODI "MODI"
#define INSTR_I2D "_I_I2D"
#define INSTR_D2I "_I_D2I"
#define INSTR_COPY "_I_COPY"	   // Se asigna el contenido de una variable a otra variable.
#define INSTR_CONSULT "_I_CONSULT" // Se asigna el contenido de una posición de un tensor a una variable.
#define INSTR_ASSIGN "_I_ASSIGN"   // Se asigna el contenido de una variable en una posición del tensor.
#define INSTR_PARAM "_I_PARAM"
#define INSTR_CALL "_I_CALL"
#define INSTR_PUT "_I_PUT"
#define INSTR_HALT "_I_HALT"
#define INSTR_RETURN "_I_RETURN"

typedef struct
{
	char *type;			 // Tipo del elemenento.
	char *value;		 // Valor del elemento.
	char *lexema;		 // Nombre de var en caso de ser ID_ARIT
	char *valueInfoType; // variable, tensor, funcion, literal
	char *index;		 // indice de acceso en caso de ser un tensor, como el indice puede estar expresado dentro de una variable;
} value_info;

typedef struct
{
	int index_dim; // Posición actual del vector de dimensiones del tensor.
	int calcIndex; // Variable sobre la que se van acumulando los cálculos parciales para acceder a la posición del tensor demandada.
	char *lexema;  // Nombre de la variable tensor.
} tensor_info;

typedef struct
{
	int dim;		// Dimension concreta que se esta evaluando actaulmente.
	char *type;		// Tipo concreto que se esta evaluando actualmente.
	void *elements; // Valores dentro del componente.
	int num_elem;	// Número de elementos del tensor.
} tensor_ini_info;

#endif
