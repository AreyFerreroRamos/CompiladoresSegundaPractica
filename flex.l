/***************** ESPECIFICACIO LEX ***************/

/*
 
*/

/***************************************************/

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h> 
  #include "bison.tab.h"
  #include "functions.h"

  extern FILE *yyin;
  extern void yyerror(char *);
%}

%option yylineno
%option noyywrap

lletra	[a-zA-Z]
digit	[0-9]


%%


({digit})+	{
			//debug("%s\n",yytext);
			yylval.enter = atoi(yytext);
			return INTEGER;
		}
	
({digit})+\.({digit})+(e(-)?({digit})+)?	{
							//debug("%s\n",yytext);
							yylval.real = atof(yytext);
							return FLOAT;						
						}

\"(.)*\"	{
			//debug("%s\n",yytext);
			yylval.cadena = yytext;
			return STRING;
		}

(true|false)	{
			//debug("%s\n",yytext);
			yylval.boolea = strcmp(yytext,"true") ? false : true;
			return BOOLEAN;
		}

(#(.)*|#=(.|\n)*=#) 	{
				//debug("COMMENT: %s\n",yytext);
			}

{lletra}({lletra}|{digit}|_({lletra}|{digit}))*	{
							//debug("%s\n",yytext);
							yylval.ident.lexema = strncpyWithSentinel(yyleng,yytext);
							yylval.ident.lenght = yyleng;
							yylval.ident.line = yylineno;
							return ID;
						}

":="	{
		//debug("%s\n",yytext);
		return ASSIGN;
	}

\^	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return OP_ARIT_P1;
	}

(\*|\/|\%)	{
			//debug("%s\n",yytext);
			yylval.cadena = strncpyWithSentinel(yyleng,yytext);
			return OP_ARIT_P2;
		}

(\+|\-)	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return OP_ARIT_P3;
	}

(">"|">="|"=="|"<="|"<"|"!=")	{
					//debug("%s\n",yytext);
					yylval.cadena = strncpyWithSentinel(yyleng,yytext);
					return OP_RELACIONAL;
				}

(&&|"||")	{
			//debug("%s\n",yytext);
			yylval.cadena = strncpyWithSentinel(yyleng,yytext);
			return OP_BOOL;
		}

"!"	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return NEGACION;
	}

"("	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return PARENTESIS_ABIERTO;
	}

")"	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return PARENTESIS_CERRADO;
	}

.	{}


%%
