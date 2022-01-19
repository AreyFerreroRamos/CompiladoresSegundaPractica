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

  /* CONTROL FOR FUNCTION DEFINITION */
  bool inFunction =false;
  bool inHeader = false;
  bool inParams = false;
  bool idProcReaded=false;
  bool isAction = true;
  bool isReturnLine = false;
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
        if(inHeader==true){
            inParams=true;
        }
		debug("%s\n", yytext, 0);
		return PARENTESIS_ABIERTO;
	}

")" {
        if(inHeader==true){
            inHeader=false;
            inParams=false;
        }
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

"{" {
        debug("%s\n", yytext, 0);
        return LLAVE_ABIERTA;
    }

"}" {
        debug("%s\n", yytext, 0);
        return LLAVE_CERRADA;
    }


;   {
		debug("%s\n", yytext, 0);
		return PUNTO_Y_COMA;
	}

::  {
        if(inFunction ==true && inParams == false){
            isAction = false;
        }
        debug("%s\n", yytext, 0);
        return DOBLE_DOS_PUNTOS;
    }

"function"  {
                debug("%s\n", yytext, 0);
                pushSymtab();
                inFunction =true;
                inHeader = true;
                return START;
            }

"return"    {

		        isReturnLine = true;
                if(isAction ==true){
                    debug("%s\n", yytext, 0);
                    return DIRECTRETURN;
                }else{
                    debug("%s with action\n", yytext, 0);
                    return VALUERETURN;
                }
            }

"end"	{
            debug("%s\n", yytext, 0);
            inFunction =false;
            inParams = false;
            idProcReaded = false;
            isAction=true;
            return END;
        }

("Int32"|"Float64"|"String"|"Bool"|"Tensor") {
                                                debug("%s\n", yytext, 0);
                                                yylval.cadena =strdup(yytext);
                                                return TYPE;
                                            }

{id}	{
            if (strlen(yytext) > ID_MAX_LENGTH)
            {
                yyerror(generateString("ID -> %s demasiado largo", 1, yytext));
            }
            else if (assignFounded == false && corcheteOpenFounded == false && isReturnLine == false && ( (inFunction == true && idProcReaded == true) || inFunction == false))
            {
                debug("ID POINTER: %s\n", yytext, 0);
                yylval.ident.lexema = strdup(yytext);
                yylval.ident.lenght = strlen(yytext);
                yylval.ident.line = yylineno;
                return ID;
            }
            else if (inHeader == true && idProcReaded == false)
            {
                debug("ID PROC: %s\n", yytext, 0);
                idProcReaded=true;
                yylval.cadena = strdup(yytext);
                return ID_PROC;
            }
            else
            {
                debug("ID ARIT: %s\n", yytext, 0);
                sym_value_type entry = getEntry(yytext);
                value_info v = createValueInfo(yytext, entry.type, entry.entryType, generateEmptyValueInfoBase());
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
		isReturnLine = false;
	}

.	{}


%%
