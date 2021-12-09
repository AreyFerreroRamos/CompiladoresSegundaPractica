#ifndef TIPUS_H

#define TIPUS_H

#define STR_MAX_LENGTH 200
#define INT_MAX_LENGTH_STR 10
#define FLOAT_MAX_LENGTH_STR 20

#define INT32_T "Int32"
#define FLOAT64_T "Float64"
#define BOOLEAN_T "Boolean"
#define STRING_T "String"

#define IDENT_T "Ident"

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

typedef struct
{
	char *type;
	char *value;
} value_info;

typedef struct
{
	int index_dim;
	int calcIndex;
	char *lexema;
} tensor_info;

typedef struct
{
	int dim;		// Dimension concreta que se esta evaluando actaulmente.
	char *type;		// Tipo concreto que se esta evaluando actualmente.
	void *elements; // Valores dentro del componente.
	int num_elem;
} tensor_ini_info;

#endif
