#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions2.h"

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;

extern int num_tmp_variable;

char *generateTmpId()
{
	char *id;
	
	id = (char *) malloc(sizeof(char) * TMP_ID_MAX_LENGTH);
	sprintf(id, "%s%i", TMP_BASE_ID, num_tmp_variable);
	num_tmp_variable++;
	return id;
}

void emet(char *type, value_info v1, value_info v2, value_info v3)
{
	
}
