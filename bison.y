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
%token <cadena> STRING OP_ARIT_P1 OP_ARIT_P2 OP_ARIT_P3 OP_RELACIONAL OP_BOOL NEGACION PARENTESIS_ABIERTO PARENTESIS_CERRADO
%token <boolea> BOOLEAN
%token <ident> ID

%type <operand> expresion_aritmetica lista_sumas lista_productos lista_potencias expresion_booleana expresion_booleana_base literal

%start programa


%%


programa : lista_de_sentencias

lista_de_sentencias : lista_de_sentencias sentencia | sentencia

sentencia : asignacion 
	| expresion_aritmetica 
	| expresion_booleana 	{
					char * boolValue = isTrue(atoi($1.value)) ? "true" : "false";
					fprintf(yyout, "La expresion booleana es %s",boolValue);
				}

asignacion : ID ASSIGN expresion_aritmetica	{	
		   					debug("id:%s\n",$1.lexema);
							debug("valor:%s\n",$3.value);
							fprintf(yyout, "ID: %s pren per valor: %s",$1.lexema, $3.value);
						}
	| ID ASSIGN expresion_booleana	{

					}

expresion_aritmetica : lista_sumas

lista_sumas : lista_sumas OP_ARIT_P3 lista_productos	{
								if(isNumberType($3.type))
								{
									debug("operand: %s\n", $2);
									$$.value = (char *) malloc(sizeof(char)*FLOAT_MAX_LENGTH_STR);
									if(!doOperationAritmetic($1,$2,$3,&$$))
									{
										yyerror("Something wrong with operation");
									}
									debug("valor: %s\n",$$.value);

								}
								else
								{
									char * error = allocateSpaceForMessage();
									sprintf(error,"Cannot do operation with type %s",$3.type);
									yyerror(error);
								} 
							}	
		| lista_productos	{ 	
						if(isNumberType($1.type))
						{
							$$ = createValueInfo(FLOAT_MAX_LENGTH_STR,$1.value,$1.type);
						}
						else
						{
							char * error = allocateSpaceForMessage();
							sprintf(error,"Cannot do operation with type %s",$1.type);
							yyerror(error);
						}
					}

lista_productos : lista_productos OP_ARIT_P2 lista_potencias 	{
									if (isNumberType($3.type))
									{
										debug("operand: %s\n", $2);
										$$.value = (char *) malloc(sizeof(char)*STR_MAX_LENGTH);
										if (!doOperationAritmetic($1, $2, $3, &$$))
										{
											yyerror("Something wrong with operation.");
										}
										debug("Valor: %s\n", $$.value);
									}
									else
									{
										char * error = allocateSpaceForMessage();
										sprintf(error, "Cannot do operation with %s", $3.value);
										yyerror(error);
									}
								}		
		| lista_potencias	{
						if(isNumberType($1.type))
						{
							$$ = createValueInfo(FLOAT_MAX_LENGTH_STR,$1.value,$1.type);
						}
						else
						{
							char * error = allocateSpaceForMessage();
							sprintf(error, "Cannot do operation with %s", $1.value);
							yyerror(error);
						}
					}

lista_potencias : lista_potencias OP_ARIT_P1 literal	{
								if (isNumberType($3.type))
								{
									debug("operand: %s\n", $2);
									$$.value = (char *) malloc(sizeof(char)*STR_MAX_LENGTH);
									if (!doOperationAritmetic($1, $2, $3, &$$))
									{
										yyerror("Something wrong with operation.");
									}
									debug("Valor: %s\n", $$.value);
								}
								else
								{
									char * error = allocateSpaceForMessage();
									sprintf(error, "Cannot do operation with %s", $3.value);
									yyerror(error);
								}
							}
		| literal	{
					if(isNumberType($1.type))
					{
						$$ = createValueInfo(FLOAT_MAX_LENGTH_STR,$1.value,$1.type);
					}
					else
					{
						char * error = allocateSpaceForMessage();
						sprintf(error, "Cannot do operation with %s", $1.value);
						yyerror(error);
					}
				}

expresion_booleana : expresion_booleana OP_BOOL expresion_booleana_base	{
										if(strcmp($2,OP_BOOL_AND)==0)
										{
											if( isFalse(atoi($1.value)) || isFalse(atoi($3.value)))
											{
												$$ = createValueInfo(1,iota(0),BOOLEAN_T);
											}
											else
											{
												$$ = createValueInfo(1,iota(1),BOOLEAN_T);
											}
										}
										else
										{
											if( isTrue(atoi($1.value)) || isTrue(atoi($3.value)))
											{
												$$ = createValueInfo(1,iota(1),BOOLEAN_T);
											}
											else
											{
												$$ = createValueInfo(1,iota(0),BOOLEAN_T);
											}
										}
									}
		| expresion_booleana_base	{ 
							$$ = createValueInfo(1,$1.value,BOOLEAN_T);
						}
		| NEGACION expresion_booleana_base	{
								int res = negateBoolean(atoi($2.value));
								$$ = createValueInfo(1,iota(res),BOOLEAN_T);
							}

expresion_booleana_base : literal OP_RELACIONAL literal {
														if(isNumberType($1.type) && isNumberType($3.type) && isSameType($1.type,$3.type))
														{
															int res = doRelationalOperation(atof($1.value),$2,atof($3.value));
															$$ = createValueInfo(1,iota(res),BOOLEAN_T);
														}
														else
														{
															char * error = allocateSpaceForMessage();
															sprintf(error,"Cannot do comparation %s %s %s",$1.value,$2,$3.value);
															yyerror(error);
														}
													}
					| literal {
						if(isSameType($1.type,IDENT_T))
						{
							//Se deberia crear con la info en symtab
							$$ = createValueInfo(strlen($1.value),$1.value,$1.type);
						}
						else if (isSameType($1.type,BOOLEAN_T))
						{
							$$ = createValueInfo(1,$1.value,$1.type);
						}
						else
						{
							char * error = allocateSpaceForMessage();
							sprintf(error,"%s is not valid for boolean expression",$1.value);
							yyerror(error);
						}
					}

literal : INTEGER	{ 	
				$$ = createValueInfo(INT_MAX_LENGTH_STR,iota($1),INT32_T);
			}
	| FLOAT		{
				$$ = createValueInfo(FLOAT_MAX_LENGTH_STR,fota($1),FLOAT64_T);
			}
	| STRING	{
				$$ = createValueInfo(strlen($1),$1,STRING_T);
			}
	| BOOLEAN	{	
				$$ = createValueInfo(1,iota($1),BOOLEAN_T);
			}
	| ID	{
			//Se deberia crear con la info en symtab
			$$ = createValueInfo(strlen($1.lexema),$1.lexema,IDENT_T);
		}
	| PARENTESIS_ABIERTO lista_sumas PARENTESIS_CERRADO	{
									if (isNumberType($2.type))
									{
										$$ = createValueInfo(FLOAT_MAX_LENGTH_STR, $2.value, $2.type);
									}
									else 
									{
										char * error = allocateSpaceForMessage();
										sprintf(error, "Cannot do operation with %s", $2.value);
										yyerror(error);	
									}	
								}
	| PARENTESIS_ABIERTO expresion_booleana PARENTESIS_CERRADO	{


									}


%%
