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

  extern void yyerror(char *);
  bool assignFounded = false;
  bool corcheteOpenFounded = false;
%}

%option yylineno
%option noyywrap

lletra	[a-zA-Z]
digit	[0-9]
id {lletra}({lletra}|{digit}|_({lletra}|{digit}))*


%%


({digit})+	{
			debug("%s\n", yytext, 0);
			yylval.enter = atoi(yytext);
			return INTEGER;
		}

({digit})+"."({digit})+(e(-)?({digit})+)?	{
							debug("%s\n", yytext, 0);
							yylval.real = atof(yytext);
							return FLOAT;						
						}

(true|false)	{
			debug("%s\n", yytext, 0);
			yylval.boolea = strcmp(yytext,"true") ? false : true;
			return BOOLEAN;
		}

(#(.)*|#=(.|\n)*=#) 	{
				debug("COMMENT: %s\n",yytext,0);
			}

"^"	{
		debug("%s\n", yytext, 0);
		yylval.cadena = strdup(yytext);
		return OP_ARIT_P1;
	}

("/"|"%")	{
			debug("%s\n", yytext, 0);
			yylval.cadena = strdup(yytext);
			return OP_ARIT_P2;
		}

"*"	{
		debug("%s\n", yytext, 0);
		yylval.cadena = strdup(yytext);
		return ASTERISCO;
	}

("+"|"-")	{
			debug("%s\n", yytext, 0);
			yylval.cadena = strdup(yytext);
			return OP_ARIT_P3;
		}

(">"|">="|"=="|"<="|"<"|"!=")	{
					debug("%s\n", yytext, 0);
					yylval.cadena = strdup(yytext);
					return OP_RELACIONAL;
				}

(&&|"||")	{
			debug("%s\n", yytext, 0);
			yylval.cadena = strdup(yytext);
			return OP_BOOL;
		}

"!"	{
		debug("%s\n", yytext, 0);
		return NEGACION;
	}

"("	{
		debug("%s\n", yytext, 0);
		return PARENTESIS_ABIERTO;
	}

")"	{
		debug("%s\n", yytext, 0);
		return PARENTESIS_CERRADO;
	}

,	{
		debug("%s\n", yytext, 0);
		return COMA;
	}

"div("	{
		debug("%s\n", yytext, 0);
		return DIV;
	}

"length("	{
			debug("%s\n", yytext, 0);
			return LENGTH;
		}	


"="	{
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

;	{
		debug("%s\n", yytext, 0);
		return PUNTO_Y_COMA;
	}

{id}	{
		if (assignFounded == false && corcheteOpenFounded == false)
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
			//TODO MIRAR PORQUE PETA AQUI AL HACER ULTIMO CASO DE PRUEBAS
			sym_value_type entry;
			int response =  sym_lookup(yytext, &entry);
			if(response==SYMTAB_OK)
			{
				value_info v = createValueInfo(strlen((char*)entry.value), (char*)entry.value, (char*)entry.type);
				if(isSameType(v.type,INT32_T) || isSameType(v.type,FLOAT64_T))
				{
					yylval.ident.lexema = strdup(v.value);
					yylval.ident.lenght = strlen(v.value);
					yylval.ident.line = yylineno;
					return ID_ARIT;
				}
				else if(isSameType(v.type,STRING_T))
				{
					yylval.cadena = v.value;
					return STRING;
				}
				else
				{
					yylval.boolea = atoi(v.value);
					return BOOLEAN;
				}
			}
			else
			{
				char * error = allocateSpaceForMessage();
				sprintf(error, "The id %s is not initialized", yytext);
				yyerror(error);	
			}
		}
		
	}

\"(\\.|[^\"])*\"	{
				debug("%s\n",yytext,0);
				yylval.cadena = strdup(yytext);
				return STRING;
			}

\n	{
		debug("%s\n",yytext,0);
		assignFounded = false;
		corcheteOpenFounded = false;
	}

.	{}


%%
