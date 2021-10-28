%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include "functions.h"

  #define YYLMAX 100

  extern FILE *yyout;
  extern int yylineno;
  extern int yylex();
  extern void yyerror(char *);
%}

%union {
	struct {
		char *lexema;
		int lenght;
		int line;
	} ident;
	int enter;
	float real;
	char *cadena;
	bool boolea;
	void *no_definit;
}

%token <no_definit> ASSIGN
%token <enter> INTEGER
%token <real> FLOAT
%token <cadena> STRING
%token <boolea> BOOLEAN
%token <ident> ID

%type <no_definit> expressio


%%


expressio :	ID	{
				fprintf(yyout,"ID: %s en la linea:  %d. La longitud es: %d",$1.lexema, $1.line, $1.lenght);			  		
	 		}
	|	ASSIGN	{
				fprintf(yyout,"ASSIGN: %s\n",$1.no_definit);
			}
	|	INTEGER {
				fprintf(yyout,"INTEGER: %d\n",$1.enter);
			}
	|	FLOAT	{
				fprintf(yyout,"FLOAT: %.2f\n",$1.real);
			}
	|	STRING	{
				fprintf(yyout,"STRING: %s\n",$1.cadena);
			}
	|	BOOLEAN	{
				fprintf(yyout,"BOOLEA: %b\n",$1.boolea);
			}


%%


int init_analisi_sintactic (char* filename)
{
	int error = EXIT_SUCCESS;

	yyout = fopen(filename,"w");

	if (yyout == NULL) {
		error = EXIT_FAILURE;
	}
	return error;
}

int analisi_semantic()
{
	int error;

	if (yyparse() == 0) {
		error =  EXIT_SUCCESS;
	}
	else {
		error =  EXIT_FAILURE;
	}
	return error;
}

int end_analisi_sintactic()
{
	int error;

	error = fclose(yyout);

	if (error == 0) {
 		error = EXIT_SUCCESS;
	}
	else {
		error = EXIT_FAILURE;	
	}
	return error;
}

void yyerror(char *explanation)
{
	fprintf(stderr,"Error: %s ,in line %d \n",explanation,yylineno);
}
