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
  #include "utils.h"

  extern void yyerror(char *);
  bool assignFounded = false;
  bool corcheteOpenFounded = false;
  bool insideFunction = false;
%}

%option yylineno
%option noyywrap

lletra	[a-zA-Z]
digit	[0-9]
id {lletra}({lletra}|{digit}|_({lletra}|{digit}))*


%%


({digit})+  {
                debug("%s\n", yytext, 0);
                yylval.enter = atoi(yytext);
                return INTEGER;
            }

({digit})+"."({digit})+(e(-)?({digit})+)?   {
                                                debug("%s\n", yytext, 0);
                                                yylval.real = atof(yytext);
                                                return FLOAT;
                                            }

(#(.)*|#=(.|\n)*=#) {
                        debug("COMMENT: %s\n", yytext, 0);
                    }

"^" {
        debug("%s\n", yytext, 0);
        yylval.cadena = strdup(yytext);
        return OP_ARIT_P1;
    }

("/"|"%")   {
                debug("%s\n", yytext, 0);
                yylval.cadena = strdup(yytext);
                return OP_ARIT_P2;
            }

"*" {
		debug("%s\n", yytext, 0);
		yylval.cadena = strdup(yytext);
		return ASTERISCO;
	}

("+"|"-")   {
                debug("%s\n", yytext, 0);
                yylval.cadena = strdup(yytext);
                return OP_ARIT_P3;
            }

"(" {
		debug("%s\n", yytext, 0);
		return PARENTESIS_ABIERTO;
	}

")" {
		debug("%s\n", yytext, 0);
		return PARENTESIS_CERRADO;
	}

,	{
		debug("%s\n", yytext, 0);
		return COMA;
	}

"div("  {
            debug("%s\n", yytext, 0);
            return DIV;
        }

"=" {
		debug("%s\n", yytext, 0);
		assignFounded = true;
		return ASSIGN;
	}

"["	{
		debug("%s\n", yytext, 0);
		corcheteOpenFounded = true;
		return CORCHETE_ABIERTO;
	}

"]"	{
		debug("%s\n", yytext, 0);
		return CORCHETE_CERRADO;
	}

;   {
		debug("%s\n", yytext, 0);
		return PUNTO_Y_COMA;
	}

"function"  {
                /*debug("%s\n", yytext, 0);
                insideFunction = true;
                return START;*/
            }

"end"	{
            /*debug("%s\n", yytext, 0);
            insideFunction = false;
            return END;*/
        }

{id}	{
            if (strlen(yytext) > ID_MAX_LENGTH)
            {
                yyerror(generateString("ID -> %s demasiado largo", 1, yytext));
            }
            else if (assignFounded == false && corcheteOpenFounded == false)
            {
                debug("ID POINTER: %s\n", yytext, 0);
                yylval.ident.lexema = strdup(yytext);
                yylval.ident.lenght = strlen(yytext);
                yylval.ident.line = yylineno;
                return ID;
            }
            else
            {
                debug("ID ARIT: %s\n", yytext, 0);
                sym_value_type entry = getEntry(yytext);
                value_info v = createValueInfo(yytext, entry.type, entry.entryType, NULL);
                if (isSameType(v.type, INT32_T) || isSameType(v.type, FLOAT64_T))
                {
                    yylval.operand = v;
                    return ID_ARIT;
                }
            }
        }

\n	{
		debug("%s\n", yytext, 0);
		assignFounded = false;
		corcheteOpenFounded = false;
	}

.	{}


%%
