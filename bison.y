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

%token <no_definit> ASSIGN OP_ARITMETIC OP_RELACIONAL OP_BOOL 
%token <enter> INTEGER
%token <real> FLOAT
%token <cadena> STRING
%token <boolea> BOOLEAN
%token <ident> ID

%start llista_de_sentencies

%%

llista_de_sentencies : llista_de_sentencies sentencia | sentencia

sentencia : assignacio | expressio_aritmetica | expressio_booleana 

assignacio : ID ASSIGN expressio_aritmetica | ID ASSIGN expressio_booleana

expressio_aritmetica : expressio_aritmetica OP_ARITMETIC literal | literal

expressio_booleana : expressio_booleana OP_BOOL condicional | condicional

condicional : INTEGER OP_RELACIONAL INTEGER | FLOAT OP_RELACIONAL FLOAT

literal :	INTEGER {
				fprintf(yyout,"INTEGER: %d",$1);
			}
	|	FLOAT	{
				fprintf(yyout,"FLOAT: %.8f",$1);
			}
	|	STRING	{
				fprintf(yyout,"STRING: %s",$1);
			}

%%
