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
id {lletra}({lletra}|{digit}|_({lletra}|{digit}))*


%%


({digit})+	{
			//debug("%s\n",yytext);
			yylval.enter = atoi(yytext);
			return INTEGER;
		}

({digit})+"."({digit})+(e(-)?({digit})+)?	{
							//debug("%s\n",yytext);
							yylval.real = atof(yytext);
							return FLOAT;						
						}

(true|false)	{
			//debug("%s\n",yytext);
			yylval.boolea = strcmp(yytext,"true") ? false : true;
			return BOOLEAN;
		}

(#(.)*|#=(.|\n)*=#) 	{
				//debug("COMMENT: %s\n",yytext);
			}

"^"	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return OP_ARIT_P1;
	}

("/"|"%")	{
			//debug("%s\n",yytext);
			yylval.cadena = strncpyWithSentinel(yyleng,yytext);
			return OP_ARIT_P2;
		}

"*"	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return ASTERISCO;
	}

("+"|"-")	{
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

,	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return COMA;
	}

"div("	{
		//debug("%s\n",yytext);
		yylval.cadena = strncpyWithSentinel(yyleng,yytext);
		return DIV;
	}

"length("	{
			//debug("%s\n",yytext);
			yylval.cadena = strncpyWithSentinel(yyleng,yytext);
			return LENGTH;
		}	


{id}(" ")*"="	{
					char * idName = getIdName(yytext);
					//debug("%s\n",idName);
					yylval.ident.lexema = strdup(idName);
					yylval.ident.lenght = strlen(idName);
					yylval.ident.line = yylineno;
					return IDWITHASSIGN;
				}

{id}	{
			sym_value_type entry;
			int response =  sym_lookup(yytext, &entry);
			if(response==SYMTAB_OK)
			{
				value_info v = createValueInfo(strlen((char*)entry.value),(char*)entry.value,(char*)entry.type);
				if(isSameType(v.type,INT32_T))
				{
					yylval.enter = atoi(v.value);
					return INTEGER;
				}
				else if(isSameType(v.type,FLOAT64_T))
				{
					yylval.real = atof(v.value);
					return FLOAT;
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

\"(\\.|[^\"])*\"	{
			debug("%s\n",yytext);
			yylval.cadena = strncpyWithSentinel(yyleng,yytext);
			return STRING;
		}

.	{}


%%
