%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>

  #define YYLMAX 100

  extern FILE *yyout;
  extern int yylex();
  extern void yyerror(char *);
  int *vector_dims_tensor;
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
	tensor_info tensor_info;
	tensor_ini_info tensor_ini_info;
	sym_value_type sym_value_type;
	void *no_definit;
}

%token <no_definit> ASSIGN
%token <enter> INTEGER
%token <real> FLOAT
%token <cadena> STRING OP_ARIT_P1 OP_ARIT_P2 ASTERISCO OP_ARIT_P3 OP_RELACIONAL OP_BOOL NEGACION PARENTESIS_ABIERTO PARENTESIS_CERRADO DIV LENGTH COMA CORCHETE_ABIERTO CORCHETE_CERRADO PUNTO_Y_COMA
%token <boolea> BOOLEAN
%token <ident> ID ID_ARIT

%type <operand> expresion_aritmetica lista_sumas lista_productos lista_potencias expresion_booleana expresion_booleana_base literal_aritmetic literal_boolea id_arit
%type <tensor_info> id lista_indices lista_indices_arit
%type <tensor_ini_info> tensor componente lista_componentes lista_valores
%type <cadena> op_arit_p2 concatenacion

%start programa


%%


programa : lista_de_sentencias

lista_de_sentencias : lista_de_sentencias sentencia | sentencia

sentencia : asignacion 
	| expresion_aritmetica 	{
					fprintf(yyout, "El resultado es %s\n",$1.value);
				} 
	| expresion_booleana 	{
					char * boolValue = atoi($1.value) ? "true" : "false";
					fprintf(yyout, "La expresion booleana es %s\n",boolValue);
				}

asignacion : ID ASSIGN expresion_aritmetica	{
							sym_value_type entry;
							entry.type = $3.type;
							entry.value = $3.value;
							entry.size = strlen($3.value);
							entry.num_dim = 0;
							entry.elem_dims = NULL;
							if (sym_enter($1.lexema, &entry) != SYMTAB_OK)
							{
								yyerror("Error al guardar en symtab.");
							}
							fprintf(yyout, "ID: %s pren per valor: %s\n",$1.lexema, entry.value);
						}
	| id ASSIGN expresion_aritmetica	{	
							sym_value_type entry;
							entry.type = $3.type;
							entry.value = $3.value;
							entry.size = strlen($3.value);
							entry.num_dim = 0;
							entry.elem_dims = NULL;
							if (sym_enter($1.lexema, &entry) != SYMTAB_OK)
							{
								yyerror("Error al guardar en symtab.");
							}
							fprintf(yyout, "ID: %s pren per valor: %s\n",$1.lexema, entry.value);
						}
	| ID ASSIGN expresion_booleana	{
						sym_value_type entry;
						entry.type = $3.type;
						entry.value = $3.value;
						entry.size = 1;
						entry.num_dim = 0;
						entry.elem_dims = NULL;
						if (sym_enter($1.lexema, &entry) != SYMTAB_OK)
						{
							yyerror("Error al guardar en symtab.");
						}
						fprintf(yyout, "ID: %s pren per valor: %s\n", $1.lexema, entry.value);
					}
	| ID ASSIGN concatenacion	{
						sym_value_type entry;
						entry.type = STRING_T;
						entry.value = $3;
						entry.size = strlen($3);
						entry.num_dim = 0;
						entry.elem_dims = NULL;
						if (sym_enter($1.lexema, &entry) != SYMTAB_OK)
						{
							yyerror("Error al guardar en symtab.");
						}
						fprintf(yyout, "ID: %s pren per valor: %s\n", $1.lexema, entry.value);
					}
	| ID ASSIGN tensor	{		
					sym_value_type entry;
					entry.type = $3.type;
					entry.value = NULL;
					entry.size = calculateSizeType($3.type)*sizeof($3.elements);
					entry.num_dim = $3.dim;
					entry.elem_dims = convert_invert_vector(vector_dims_tensor,$3.dim);
					entry.elements = $3.elements;
					if (sym_enter($1.lexema, &entry) != SYMTAB_OK)
					{
						yyerror("Error al guardar en symtab.");
					}
					fprintf(yyout, "ID: %s pren per valor: %s\n", $1.lexema, entry.elements);	// Refinar como se realizará la escritura en fichero.
				}

id : lista_indices CORCHETE_CERRADO	{
						$$ = createTensorInfo($1.index_dim, $1.calcIndex, $1.lexema);
					}
   
lista_indices : lista_indices COMA lista_sumas	{
							if (isSameType($3.type, INT32_T)) 
							{
								int dim = getDim($1.lexema, $1.index_dim);
								if (dim != -1) 
								{
									$$ = createTensorInfo($1.index_dim + 1, $1.calcIndex * dim + atoi($3.value), $1.lexema);
								}
								else
								{
									yyerror("Array out of bound.");
								}
							}
							else
							{
								char * error = allocateSpaceForMessage();
								sprintf(error,"Index %s is not type Int32", $3.type);
								yyerror(error);
							}
						}
		| ID CORCHETE_ABIERTO lista_sumas	{
		      						if (isSameType($3.type, INT32_T)) 
								{
									$$ = createTensorInfo(1, atoi($3.value), $1.lexema);
								}
								else
								{
									char * error = allocateSpaceForMessage();
									sprintf(error,"Index %s is not type Int32",$3.type);
									yyerror(error);
								}	
		     					}

concatenacion : concatenacion ASTERISCO STRING 	{
							$$ = allocateSpaceForMessage(strlen($1)+strlen($3)-2);
							char * var = allocateSpaceForMessage(strlen($1));
							memcpy(var,&$1[0],strlen($1));
							strcat($$,var);
							free(var);
							var = allocateSpaceForMessage(strlen($3));
							memcpy(var,&$3[1],strlen($3));
							strcat($$,var);
						}
		| STRING 	{
					$$ = strdup($1);
				}

expresion_aritmetica : lista_sumas

lista_sumas : lista_sumas OP_ARIT_P3 lista_productos	{
								if(isNumberType($3.type))
								{
									$$.value = (char *) malloc(sizeof(char)*FLOAT_MAX_LENGTH_STR);
									if(!doAritmeticOperation($1,$2,$3,&$$))
									{
										yyerror("Something wrong with operation 1");
									}
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

lista_productos : lista_productos op_arit_p2 lista_potencias 	{
									if (isNumberType($3.type))
									{
										$$.value = (char *) malloc(sizeof(char)*FLOAT_MAX_LENGTH_STR);
										if (!doAritmeticOperation($1, $2, $3, &$$))
										{
											yyerror("Something wrong with operation.");
										}
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

op_arit_p2: OP_ARIT_P2	{
				$$ = strdup($1);
			} 
	| ASTERISCO	{
				$$ = strdup($1);
			}

lista_potencias : lista_potencias OP_ARIT_P1 literal_aritmetic	{
									if (isNumberType($3.type))
									{
										$$.value = (char *) malloc(sizeof(char)*FLOAT_MAX_LENGTH_STR);
										if (!doAritmeticOperation($1, $2, $3, &$$))
										{
											yyerror("Something wrong with operation.");
										}
									}
									else
									{
										char * error = allocateSpaceForMessage();
										sprintf(error, "Cannot do operation with %s", $3.value);
										yyerror(error);
									}
								}
		| literal_aritmetic	{
						if(isNumberType($1.type))
						{
							$$ = createValueInfo(FLOAT_MAX_LENGTH_STR, $1.value, $1.type);
						}
						else
						{
							char * error = allocateSpaceForMessage();
							sprintf(error, "Cannot do operation with %s", $1.value);
							yyerror(error);
						}
					}

literal_aritmetic : INTEGER	{ 	
					$$ = createValueInfo(INT_MAX_LENGTH_STR, iota($1), INT32_T);
				}
	| FLOAT		{
				$$ = createValueInfo(FLOAT_MAX_LENGTH_STR, fota($1), FLOAT64_T);
			}
	| id_arit 	{
				$$ = createValueInfo(FLOAT_MAX_LENGTH_STR, $1.value, $1.type);
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
	| DIV lista_sumas COMA lista_sumas PARENTESIS_CERRADO	{
									if ((isNumberType($2.type)) && (isNumberType($4.type)))
									{
										$$.value = (char *) malloc(sizeof(char)*FLOAT_MAX_LENGTH_STR);
										if(!doAritmeticOperation($2, "/", $4, &$$))
										{
											yyerror("Something wrong with operation 2");
										}
									}
									else
									{
										char * error = allocateSpaceForMessage();
										sprintf(error,"Cannot do operation with type %s", $2.type);
										yyerror(error);
									} 
								}
	| LENGTH STRING PARENTESIS_CERRADO	{
							$$ = createValueInfo(INT_MAX_LENGTH_STR, iota(lenght($2)), INT32_T);
						}

id_arit : ID_ARIT	{
				sym_value_type res;
				sym_lookup($1.lexema, &res);
				$$ = createValueInfo(FLOAT_MAX_LENGTH_STR,res.value, res.type);	
       			}	
	| lista_indices_arit CORCHETE_CERRADO	{
							sym_value_type res;
							sym_lookup($1.lexema, &res);
							void * elem = res.elements;
							if (isSameType(res.type, INT32_T))
							{
								$$.value = iota(((int *) res.elements)[$1.calcIndex]);
							}
							else
							{
								$$.value = fota(((float *) res.elements)[$1.calcIndex]);
							}
							$$.type = res.type;
						}

lista_indices_arit : lista_indices_arit COMA lista_sumas	{
									if (isSameType($3.type, INT32_T)) 
									{
										int dim = getDim($1.lexema, $1.index_dim);
										if (dim != -1) 
										{
											$$ = createTensorInfo($1.index_dim + 1, $1.calcIndex * dim + atoi($3.value), $1.lexema);
										}
										else 
										{
											yyerror("Array out of bound.");
										}
									}
									else
									{
										char * error = allocateSpaceForMessage();
										sprintf(error,"Index %s is not type Int32", $3.type);
										yyerror(error);
									}					
								} 
		| ID_ARIT CORCHETE_ABIERTO lista_sumas	{
	   							if (isSameType($3.type, INT32_T)) 
								{
									$$ = createTensorInfo(1, atoi($3.value), $1.lexema);
								}
								else
								{
									char * error = allocateSpaceForMessage();
									sprintf(error,"Index %s is not type Int32", $3.type);
									yyerror(error);
								}
							}

expresion_booleana : expresion_booleana OP_BOOL expresion_booleana_base	{
										if(strcmp($2,OP_BOOL_AND) == 0)
										{
											if( ! atoi($1.value) || ! atoi($3.value))
											{
												$$ = createValueInfo(1, iota(0), BOOLEAN_T);
											}
											else
											{
												$$ = createValueInfo(1, iota(1), BOOLEAN_T);
											}
										}
										else
										{
											if( atoi($1.value) || atoi($3.value))
											{
												$$ = createValueInfo(1, iota(1), BOOLEAN_T);
											}
											else
											{
												$$ = createValueInfo(1, iota(0), BOOLEAN_T);
											}
											
										}
									}
		| NEGACION expresion_booleana_base	{
								int res = negateBoolean(atoi($2.value));
								$$ = createValueInfo(1, iota(res), BOOLEAN_T);
							}
		| expresion_booleana_base	{ 
							$$ = createValueInfo(1, $1.value, BOOLEAN_T);
						}

expresion_booleana_base : lista_sumas OP_RELACIONAL lista_sumas {
									if(isNumberType($1.type) && isNumberType($3.type) && isSameType($1.type, $3.type))
									{
										int res = doRelationalOperation(atof($1.value), $2, atof($3.value));
										$$ = createValueInfo(1, iota(res), BOOLEAN_T);
									}
									else
									{
										char * error = allocateSpaceForMessage();
										sprintf(error, "Cannot do comparation %s %s %s", $1.value, $2, $3.value);
										yyerror(error);
									}
								}
			| literal_boolea	{
							if (isSameType($1.type,BOOLEAN_T))
							{
								$$ = createValueInfo(1, $1.value, $1.type);
							}
							else
							{
								char * error = allocateSpaceForMessage();
								sprintf(error, "%s is not valid for boolean expression", $1.value);
								yyerror(error);
							}
						}

literal_boolea : BOOLEAN	{	
					$$ = createValueInfo(1, iota($1), BOOLEAN_T);
				}
		| PARENTESIS_ABIERTO expresion_booleana PARENTESIS_CERRADO	{
											if (isSameType($2.type, BOOLEAN_T))
											{
												$$ = createValueInfo(1, $2.value, BOOLEAN_T);
											}
											else
											{
												char * error = allocateSpaceForMessage();
												sprintf(error, "Cannot do operation with %s", $2.value);
												yyerror(error);
											}
										}

tensor : CORCHETE_ABIERTO lista_componentes CORCHETE_CERRADO	{
									$$.dim = $2.dim + 1;
									$$.type = $2.type;
									$$.elements = $2.elements;		
								}

lista_componentes : lista_componentes PUNTO_Y_COMA componente	{
									$$.dim = $1.dim;
									if(isSameType($1.type, INT32_T) && isSameType($3.type, INT32_T))
									{
										$$.type = INT32_T;
									}
									else
									{
										$$.type = FLOAT64_T;
									}
									$$.elements = castTensorToVoidPointer($1.elements, $1.type, $3.elements, $3.type);						
									addElementsDim(vector_dims_tensor, $1.dim);
								}
		| componente	{
					$$.dim = $1.dim;
					$$.type = $1.type;
					$$.elements = $1.elements;
					addElementsDim(vector_dims_tensor, $1.dim);
				}

componente : lista_valores	{
					$$.dim = $1.dim;
					$$.type = $1.type;
					$$.elements = $1.elements;
				}
	| tensor	{
				$$.dim = $1.dim;
				$$.type = $1.type;
				$$.elements = $1.elements;
			}

lista_valores : lista_valores COMA lista_sumas	{
							$$.dim = 0;
							if(isSameType($1.type, INT32_T) && isSameType($3.type, INT32_T))
							{
								$$.type = INT32_T;
							}
							else
							{
								$$.type = FLOAT64_T;
							}
							$$.elements = castTensorToVoidPointer($1.elements, $1.type, castValueToVoidPointer($3.value, $3.type), $3.type);
							addElementsDim(vector_dims_tensor, 0);
						}
		| lista_sumas	{
					$$.dim = 0;
					$$.type = $1.type;
					$$.elements = castValueToVoidPointer($1.value, $1.type);
					addElementsDim(vector_dims_tensor, 0);
				}


%%
