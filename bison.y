%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>

  #define YYLMAX 100

  extern FILE *yyout;
  extern int yylineno;
  extern int yylex();
  extern void yyerror(char *);
%}

%code requires {
  #include "functions.h"
  #include "tipus.h"
}

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
	value_info operand;
	void *no_definit;
}

%token <no_definit> ASSIGN  
%token <enter> INTEGER
%token <real> FLOAT
%token <cadena> STRING OP_ARIT_P1 OP_ARIT_P2 OP_ARIT_P3 OP_RELACIONAL OP_BOOL
%token <boolea> BOOLEAN
%token <ident> ID

%type <operand> literal lista_sumas

%start programa


%%


programa : lista_de_sentencias

lista_de_sentencias : lista_de_sentencias sentencia | sentencia

sentencia : asignacion | expresion_aritmetica 

asignacion : ID ASSIGN lista_sumas	{	
	   					debug("id:%s\n",$1.lexema);
						debug("valor:%s\n",$3.value);
						fprintf(yyout, "ID: %s pren per valor: %s\n",$1.lexema, $3.value);
					}

expresion_aritmetica : lista_sumas

lista_sumas : lista_sumas OP_ARIT_P3 literal	{
							if(canTypeDoOperationP3($3.type))
							{
								debug("operand: %s\n", $2);
								$$.value = (char *) malloc(sizeof(char)*STR_MAX_LENGTH);
								if(!doOperationAritmeticP3($1,$2,$3,&$$))
								{
									yyerror("Something wrong with operation");
								}
								debug("valor: %s\n",$$.value);

							}
							else
							{
								char * error;
								error = (char *) malloc(sizeof(char)*STR_MAX_LENGTH);
								sprintf(error,"Cannot do operation with type %s",$3.type);
								yyerror(error);
							} 
						}	
			| literal	{ 	
						if(canTypeDoOperationP3($1.type))
						{
							$$.value = (char *) malloc(sizeof(char)*STR_MAX_LENGTH);
							$$.value=$1.value;
							$$.type=$1.type;
						}
						else
						{
							char * error;
							error = (char *) malloc(sizeof(char)*STR_MAX_LENGTH);
							sprintf(error,"Cannot do operation with type %s",$1.type);
							yyerror(error);
						} 
					}

literal : INTEGER	{ 	
				$$.value = (char *) malloc(sizeof(char)*STR_MAX_LENGTH);
				sprintf($$.value, "%d", $1);
				$$.type="Int32";
			}
	| FLOAT		{
				sprintf($$.value, "%f", $1);
				$$.type="Float64";
			}
	| STRING	{
				sprintf($$.value, "%s", $1);
				$$.type="String";
			}
	| BOOLEAN	{	
				sprintf($$.value, "%d", $1);
				$$.type="Bool";
			}
	| ID	{
			sprintf($$.value, "%d", $1);
			$$.type="Ident";
		}


%%
