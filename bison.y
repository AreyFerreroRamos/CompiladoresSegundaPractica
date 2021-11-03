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
%token <cadena> STRING RESIDUE
%token <boolea> BOOLEAN
%token <ident> ID

%start arxiu

%%

arxiu : arxiu expressio | expressio 

expressio :	ASSIGN	{
				fprintf(yyout,"ASSIGN: %s",$1);
			}
	|	INTEGER {
				fprintf(yyout,"INTEGER: %d",$1);
			}
	|	FLOAT	{
				fprintf(yyout,"FLOAT: %.8f",$1);
			}
	|	STRING	{
				fprintf(yyout,"STRING: %s",$1);
			}
	|	BOOLEAN	{
				fprintf(yyout,"BOOLEA: %d",$1);
			}
	|	ID	{
				fprintf(yyout,"ID: %s en la linea:  %d. La longitud es: %d ",$1.lexema, $1.line, $1.lenght);			  		
	 		}

%%
