#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "functions2.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;

extern int num_tmp_variable;
extern int sq;		   // Siguiente linia que toca escribir
extern int lengthCode; // Nombre de linias de c3a
extern char **c3a;	   // Conjunto de lineas que formarán el codigo de 3 adreces

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

int init_analisi_lexic(char *file_name)
{
	int error;

	yyin = fopen(file_name, "r");

	if (yyin == NULL)
	{
		error = EXIT_FAILURE;
	}
	else
	{
		error = EXIT_SUCCESS;
	}
	return error;
}

int init_analisi_sintactic(char *filename)
{
	int error = EXIT_SUCCESS;

	yyout = fopen(filename, "w");

	if (yyout == NULL)
	{
		error = EXIT_FAILURE;
	}
	return error;
}

int analisi_semantic()
{
	int error;

	if (yyparse() == 0)
	{
		error = EXIT_SUCCESS;
	}
	else
	{
		error = EXIT_FAILURE;
	}
	return error;
}

int end_analisi_lexic()
{
	int error;

	error = fclose(yyin);

	if (error == 0)
	{
		error = EXIT_SUCCESS;
	}
	else
	{
		error = EXIT_FAILURE;
	}
	return error;
}

int end_analisi_sintactic()
{
	int error;

	error = fclose(yyout);

	if (error == 0)
	{
		error = EXIT_SUCCESS;
	}
	else
	{
		error = EXIT_FAILURE;
	}
	return error;
}

void yyerror(char *explanation)
{
	fprintf(stderr, "Error: %s, in line %d\n", explanation, yylineno);
	exit(EXIT_FAILURE);
}

char *generateTmpId()
{
	char *id;

	id = (char *)malloc(sizeof(char) * ID_MAX_LENGTH);
	sprintf(id, "%s%i", TMP_BASE_ID, num_tmp_variable);
	num_tmp_variable++;
	return id;
}

void emet(char *type, value_info v1, value_info v2, value_info v3)
{
	char instruction;
	if (isSameType(type, INSTR_COPY))
	{
		// var := var // var := 9999999999 // var := var[9999999999] // var[9999999999] := var ...
		if (isSameType(v1.valueInfoType, TENS_T))
		{
			if (isSameType(v2.valueInfoType, VAR_T))
			{
				instruction = generateString("%s[%s] := %s", 3, v1.lexema, itos(v1.index), v2.lexema);
			}
			else if (isSameType(v2.valueInfoType, TENS_T))
			{
				instruction = generateString("%s[%s] := %s[%s]", 4, v1.lexema, itos(v1.index), v2.lexema, itos(v2.index));
			}
			else if (isSameType(v2.valueInfoType, LIT_T))
			{
				instruction = generateString("%s[%s] := %s", 3, v1.lexema, itos(v1.index), v2.value);
			}
			else if (isSameType(v2.valueInfoType, FUNC_T))
			{
				instruction = generateString("%s[%s] := CALL %s", 3, v1.lexema, itos(v1.index), v2.lexema);
				// TODO añadir parámetros de la función
			}
		}
		else
		{
			if (isSameType(v2.valueInfoType, VAR_T))
			{
				instruction = generateString("%s := %s", 3, v1.lexema, v2.lexema);
			}
			else if (isSameType(v2.valueInfoType, TENS_T))
			{
				instruction = generateString("%s := %s[%s]", 4, v1.lexema, v2.lexema, itos(v2.index));
			}
			else if (isSameType(v2.valueInfoType, LIT_T))
			{
				instruction = generateString("%s := %s", 3, v1.lexema, v2.value);
			}
			else if (isSameType(v2.valueInfoType, FUNC_T))
			{
				instruction = generateString("%s := CALL %s", 3, v1.lexema, v2.lexema);
				// TODO añadir parámetros de la función
			}
		}
		writeLine(sq, instruction);
	}
}

void writeLine(int line, char *instruction)
{
	if (c3a == NULL)
	{
		c3a = malloc(sizeof(char *));
		lengthCode++;
		sq++;
	}
	else if (line > lengthCode)
	{
		c3a = realloc(c3a, (lengthCode + 1) * sizeof(char *));
		lengthCode++;
		sq++;
	}
	c3a[line] = strdup(instruction);
}

void printCode3Adresses()
{
	printf("---------------------------------");
	for (int i = 0; i < lengthCode; i++)
	{
		printf(c3a[i]);
		fprintf(yyout, c3a[i]);
	}
	printf("---------------------------------");
}

int isSameType(char *type1, char *type2)
{
	return (strcmp(type1, type2) == 0);
}

char *itos(int num)
{
	char *string = (char *)malloc(sizeof(char) * INT_MAX_LENGTH_STR);
	sprintf(string, "%i", num);
	return string;
}

char *ftos(float num)
{
	char *string = (char *)malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
	sprintf(string, "%.2f", num);
	return string;
}

char *allocateSpaceForMessage()
{
	char *message;
	message = (char *)malloc(sizeof(char) * MESSAGE_MAX_LENGTH);
	return message;
}

void debug(char *text, char *var, int typeFile)
{
	// flex
	if (typeFile == 0)
	{
		printf(text, var);
	}
	// bison
	else
	{
		// printf(text, var);
	}
}

void simpleDebug(char *text, int typeFile)
{
	// flex
	if (typeFile == 0)
	{
		// printf(text);
	}
	// bison
	else
	{
		// printf(text);
	}
}

char *generateString(char *message, int nArgs, ...)
{
	va_list ap;
	va_start(ap, nArgs);
	char **params = malloc(sizeof(char *) * nArgs);
	for (int i = 0; i < nArgs; i++)
	{
		params[i] = va_arg(ap, char *);
	}
	va_end(ap);

	char *string = allocateSpaceForMessage();
	switch (nArgs)
	{
	case 0:
		sprintf(string, message);
		break;
	case 1:
		sprintf(string, message, params[0]);
		break;
	case 2:
		sprintf(string, message, params[0], params[1]);
		break;
	case 3:
		sprintf(string, message, params[0], params[1], params[2]);
		break;
	case 4:
		sprintf(string, message, params[0], params[1], params[2], params[3]);
		break;
	case 5:
		sprintf(string, message, params[0], params[1], params[2], params[3], params[4]);
		break;
	case 6:
		sprintf(string, message, params[0], params[1], params[2], params[3], params[4], params[5]);
		break;
	case 7:
		sprintf(string, message, params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
		break;

	default:
		yyerror("ERROR. using bad function generateInstruction(),, bad nArgs (0 to 7 both included)");
	}
	return strdup(string);
}