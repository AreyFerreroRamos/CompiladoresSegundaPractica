/***************** ESPECIFICACIO LEX ***************/

/*
 
*/

/***************************************************/

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h> 
  #include "bison.tab.h"
  #include "functions.h"

  extern FILE *yyin;
%}

%option yylineno
%option noyywrap

lletra	[a-zA-Z]
digit	[0-9]


%%


":="	{
		debug("%s\n",yytext);
		return ASSIGN;
	}	

{digit}({digit})*	{
				debug("%s\n",yytext);
				yylval.enter = atoi(yytext);
				return INTEGER;
			}
	
{digit}({digit})*\.{digit}({digit})*(e(-)?{digit}({digit})*)?	{
									debug("%s\n",yytext);
									yylval.real = atof(yytext);
									return FLOAT;						
								}

\"(.)*\"	{
			debug("%s\n",yytext);
			yylval.cadena = yytext;
			return STRING;
		}

(true|false)	{
			debug("%s\n",yytext);
			yylval.boolea = strcmp("true",yytext) ? false : true;
			return BOOLEAN;
		}

(#(.)*|#=(.|\n)*=#) 	{
				debug("COMMENT: %s\n",yytext);
			}

{lletra}({lletra}|{digit}|_({lletra}|{digit}))*	{
							debug("%s\n",yytext);
							yylval.ident.lexema = (char *)malloc(sizeof(char)*yyleng);
							strncpy(yylval.ident.lexema,yytext,yyleng);
							yylval.ident.lenght = yyleng;
							yylval.ident.line = yylineno;
							return ID;
						}

.	{
		yylval.cadena = yytext;
		return RESIDUE;
	}


%%


int init_analisi_lexic(char *file_name)
{
	int error;

	yyin=fopen(file_name,"r");

	if (yyin == NULL) {
		error = EXIT_FAILURE;
	} 
	else {
		error = EXIT_SUCCESS;
	}
	return error;
}

int end_analisi_lexic()
{
	int error;

	error = fclose(yyin);

	if (error == 0){
		error = EXIT_SUCCESS;
	}
	else {
		error = EXIT_FAILURE;
	}
	return error;
}

void debug(char* text, char* var){
	printf(text, var);
}
