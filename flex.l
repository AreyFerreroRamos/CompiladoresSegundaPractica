/***************** ESPECIFICACIO LEX ***************/

/*
Exemple 7: ID := DIGIT 
*/

/***************************************************/

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h> 
  #include "bison.tab.h"

  extern FILE *yyin;
%}

%option yylineno
%option noyywrap

lletra	[a-zA-Z]
digit	[0-9]
alfanum [[:alnum:]]

%%


{lletra}({lletra}|{digit})*	{
					yylval.ident.lexema = (char *)malloc(sizeof(char)*yyleng);
					strncpy(yylval.ident.lexema,yytext,yyleng);
					yylval.ident.lenght = yyleng;
					yylval.ident.line = yylineno;
					return ID;
				}	


":="	{
		return ASSIGN;
	}	

{digit}({digit})* 	{
				yylval.enter = atoi(yytext);
				return INTEGER;
			}
	
{digit}({digit})*\.{digit}({digit})*(e(-)?{digit}({digit})*)?	{
									yylval.real = atof(yytext);
									return FLOAT;						
								}

\"(.|[[:blank:]])*\"	{
				yylval.cadena = yytext;
				return STRING;
			}

(true|false)	{
			yylval.boolea = strcmp("true",yytext) ? true : false;
			return BOOLEAN;
		}

.	{}


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
