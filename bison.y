%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>

  #define YYLMAX 100

  extern FILE *yyout;
  extern int yylex();
  extern void yyerror(char *);

  //Variables necesarias para la inicialización de un tensor
  int *vector_dims_tensor;	// Vector con el número de elementos de cada dimensión del tensor.
  int num_dims_tensor = 0;	// Número de dimensiones del tensor.
  bool *ampliar_vector_dims; 	// Vector de booleanos para limitar la ampliación de memoria del vector de dimensiones a una sola por dimensión.

  //Variables para controlar el flujo de variables temporales en la symtab
  char **list_tmp_variables_symtab;
  int num_tmp_variable = 0;

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
%token <ident> ID 
%token <operand> ID_ARIT

%type <operand> expresion_aritmetica lista_sumas lista_productos lista_potencias expresion_booleana expresion_booleana_base terminal_aritmetic terminal_boolea id_arit
%type <tensor_info> id lista_indices lista_indices_arit
%type <tensor_ini_info> tensor componente lista_componentes lista_valores
%type <cadena> op_arit_p2 concatenacion

%start programa


%%


programa : lista_de_sentencias

lista_de_sentencias : lista_de_sentencias sentencia | sentencia

sentencia : asignacion 
	| expresion_aritmetica 	{
					fprintf(yyout, "El resultado es %s\n", $1.value);
				} 
	| expresion_booleana 	{
					char * boolValue = atoi($1.value) ? "true" : "false";
					fprintf(yyout, "La expresion booleana es %s\n", boolValue);
				}
	| ID	{
			sym_value_type entry;
			int response = sym_lookup($1.lexema, &entry);
			if (response == SYMTAB_OK) 
			{
				if (entry.num_dim > 0)
				{
					printTensor($1.lexema, entry, 1);
				}
				else
				{
					fprintf(yyout, "ID: %s val:%s\n", $1.lexema, (char *) entry.value);
				}
			}
		}

asignacion : ID ASSIGN expresion_aritmetica	{
							if ($3.value != NULL) {
								sym_value_type entry;
								entry.type = $3.type;
								entry.value = $3.value;
								entry.size = strlen($3.value);
								entry.num_dim = 0;
								entry.elem_dims = NULL;
								int message = sym_enter($1.lexema, &entry);
								if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE)
								{
									yyerror("Error al guardar en symtab.");
								}
								fprintf(yyout, "ID: %s pren per valor: %s\n", $1.lexema, (char *) entry.value);
							}
							else
							{
								sym_value_type entry;
								int response = sym_lookup($3.lexema, &entry);
								if (response == SYMTAB_OK)
								{
									response = sym_enter($1.lexema, &entry);
									if (response != SYMTAB_OK && response != SYMTAB_DUPLICATE)
									{
										yyerror("Error al guardar en symtab.");
									}
									printTensor($1.lexema, entry, 1);
									clearTmpTensorId();
								}
							}
						}
	| id ASSIGN expresion_aritmetica	{	
							if ($3.value != NULL)
							{
								sym_value_type entry;
								int response = sym_lookup($1.lexema, &entry);
								if (response == SYMTAB_OK)
								{
									if (isSameType(entry.type, INT32_T))
									{	
										if (isSameType($3.type, INT32_T))
										{
											((int *) entry.elements)[$1.calcIndex] = atoi($3.value);
										}
										else
										{
											((int *) entry.elements)[$1.calcIndex] = (int) atof($3.value);
										}
									}
									else if (isSameType(entry.type, FLOAT64_T))
									{
										if (isSameType($3.type,INT32_T))
										{
											((float *) entry.elements)[$1.calcIndex] = atoi($3.value);
										}
										else
										{
											((float *) entry.elements)[$1.calcIndex] = atof($3.value);
										}
									}
									response = sym_enter($1.lexema, &entry);
									if (response != SYMTAB_OK && response != SYMTAB_DUPLICATE)
									{
										yyerror("Error al guardar en symtab.");
									}
									fprintf(yyout, "ID: %s pren per valor: %s a la posicio: %i\n", $1.lexema, (char *) $3.value, $1.calcIndex);
								}
								else
								{
									yyerror("Error al cargar variable de la symtab");
								}
							}
							else
							{
								yyerror("No se puede asignar un tensor a un indice de un tensor.");
							}
						}
	| ID ASSIGN expresion_booleana	{
						sym_value_type entry;
						entry.type = $3.type;
						entry.value = $3.value;
						entry.size = 1;
						entry.num_dim = 0;
						entry.elem_dims = NULL;
						int message = sym_enter($1.lexema, &entry);
						if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE)
						{
							yyerror("Error al guardar en symtab.");
						}
						fprintf(yyout, "ID: %s pren per valor: %s\n", $1.lexema, (char *) entry.value);
					}
	| ID ASSIGN concatenacion	{
						sym_value_type entry;
						entry.type = STRING_T;
						entry.value = $3;
						entry.size = strlen($3);
						entry.num_dim = 0;
						entry.elem_dims = NULL;
						int message = sym_enter($1.lexema, &entry);
						if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE)
						{
							yyerror("Error al guardar en symtab.");
						}
						fprintf(yyout, "ID: %s pren per valor: %s\n", $1.lexema, (char *) entry.value);
					}
	| ID ASSIGN tensor	{	
					sym_value_type entry;
					entry.type = $3.type;
					entry.value = NULL;
					entry.size = calculateSizeType($3.type) * $3.num_elem;
					entry.num_dim = $3.dim;
					invertVector(vector_dims_tensor, $3.dim);
					entry.elem_dims = vector_dims_tensor;
					entry.elements = $3.elements;
					int message = sym_enter($1.lexema, &entry);
					if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE)
					{
						yyerror("Error al guardar en symtab.");
					}
					printTensor($1.lexema, entry, 1);
					vector_dims_tensor = NULL;
					ampliar_vector_dims = NULL;
					num_dims_tensor = 0;
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
								sprintf(error, "Index %s is not type Int32", $3.type);
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
									sprintf(error, "Index %s is not type Int32", $3.type);
									yyerror(error);
								}
		     					}

concatenacion : concatenacion ASTERISCO STRING 	{
							$$ = allocateSpaceForMessage(strlen($1) + strlen($3) - 2);
							char * var = allocateSpaceForMessage(strlen($1));
							strlcpy(var, &$1[0], strlen($1));
							strcat($$, var);
							free(var);
							var = allocateSpaceForMessage(strlen($3));
							strlcpy(var, &$3[1], strlen($3));
							strcat($$, var);
						}
		| STRING 	{
					$$ = strdup($1);
				}

expresion_aritmetica : lista_sumas

lista_sumas : lista_sumas OP_ARIT_P3 lista_productos	{
								if (isNumberType($3.type))
								{	
									sym_value_type tmp;
									int response = doTensorCalcs($1.lexema, $3.lexema, $2, &tmp);
									if (response == 0)
									{
										saveTmpTensorInSymTab(&$$, $1.type, $3.type, tmp);
									}
									else if (response == -1)
									{
										yyerror("No se puede sumar un tensor con un número.");
									}
									else if (response == -2)
									{
										$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
										if (!doAritmeticOperation($1, $2, $3, &$$))
										{
											yyerror("Something wrong with operation 1.");
										}
									}
									else if (response == -3)
									{
										yyerror("Ha habido un error buscando una variable en la symtab.");
									}	
									else if (response == -4)
									{
										yyerror("No se pueden sumar o restar tensores con diferentes dimensiones.");
									}	
								}
								else
								{
									char * error = allocateSpaceForMessage();
									sprintf(error, "Cannot do operation with type %s", $3.type);
									yyerror(error);
								}
							}	
		| lista_productos	{ 	
						if (isNumberType($1.type))
						{
							$$ = createValueInfo($1.value, $1.type, $1.lexema);
						}
						else
						{
							char * error = allocateSpaceForMessage();
							sprintf(error, "Cannot do operation with type %s", $1.type);
							yyerror(error);
						}
					}

lista_productos : lista_productos op_arit_p2 lista_potencias 	{
									if (isNumberType($3.type))
									{
										int response = -4;
										sym_value_type tmp;
										if (strcmp($2, OP_ARIT_MULT) == 0)
										{
											response = doTensorProductInit($1.lexema, $3.lexema, &tmp);
										}
										if (response == 0)
										{
											int res = doTensorProductTensor($1.lexema, $3.lexema, &tmp);
											if (res != 0)
											{
												yyerror("Alguna variable no se ha encontrado en la symtab.");
											}
											else
											{
												saveTmpTensorInSymTab(&$$, $1.type, $3.type, tmp);
											}
										}
										else if (response == -1)
										{
											yyerror("Los indices de los tensores no son compatibles y no se puede realizar el producto.");
										}
										else if (response == -2)
										{
											yyerror("No esta permitido multiplicar tensores de dimensión superior a 2.");
										}
										else if (response == -3)
										{
											int res;
											if ($1.value != NULL)
											{
												res = doNumberProductTensor($1.value, $1.type, $3.lexema, &tmp);
											}
											else
											{
												res = doNumberProductTensor($3.value, $3.type, $1.lexema, &tmp);
											}
											if (res != 0)
											{
												yyerror("Alguna variable no se ha encontrado en la symtab.");
											}
											else
											{
												saveTmpTensorInSymTab(&$$, $1.type, $3.type, tmp);
											}
										}
										else if (response == -4)
										{
											if ($1.lexema == NULL && $3.lexema == NULL)
											{
												$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
												if (!doAritmeticOperation($1, $2, $3, &$$))
												{
													yyerror("Algún problema durante la operación.");
												}
											}
											else
											{
												yyerror("Los tensores no admiten la división.");
											}
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
						if (isNumberType($1.type))
						{
							$$ = createValueInfo($1.value, $1.type, $1.lexema);
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

lista_potencias : lista_potencias OP_ARIT_P1 terminal_aritmetic	{
									if (isNumberType($3.type))
									{
										$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
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
		| terminal_aritmetic	{
						if (isNumberType($1.type))
						{
							$$ = createValueInfo($1.value, $1.type, $1.lexema);
						}
						else
						{
							char * error = allocateSpaceForMessage();
							sprintf(error, "Cannot do operation with %s", $1.value);
							yyerror(error);
						}
					}

termial_aritmetic : INTEGER	{
					$$ = createValueInfo(iota($1), INT32_T, NULL);
				}
	| FLOAT		{
				$$ = createValueInfo(fota($1), FLOAT64_T, NULL);
			}
	| id_arit 	{
				$$ = createValueInfo($1.value, $1.type, $1.lexema);
			}
	| PARENTESIS_ABIERTO lista_sumas PARENTESIS_CERRADO	{
									if (isNumberType($2.type))
									{
										$$ = createValueInfo($2.value, $2.type, $2.lexema);
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
										$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
										if (!doAritmeticOperation($2, "/", $4, &$$))
										{
											yyerror("Something wrong with operation 2");
										}
									}
									else
									{
										char * error = allocateSpaceForMessage();
										sprintf(error, "Cannot do operation with type %s", $2.type);
										yyerror(error);
									}
								}
	| LENGTH STRING PARENTESIS_CERRADO	{
							$$ = createValueInfo(iota(lenght($2)), INT32_T, NULL);
						}

id_arit : ID_ARIT	{ 
				$$ = $1; 
			}	
	| lista_indices_arit CORCHETE_CERRADO	{
							sym_value_type res;
							sym_lookup($1.lexema, &res);
							if (isSameType(res.type, INT32_T))
							{
								$$.value = iota(((int *) res.elements)[$1.calcIndex]);
							}
							else
							{
								$$.value = fota(((float *) res.elements)[$1.calcIndex]);
							}
							$$.type = res.type;
							$$.lexema = NULL;
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
										sprintf(error, "Index %s is not type Int32", $3.type);
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
									sprintf(error, "Index %s is not type Int32", $3.type);
									yyerror(error);
								}
							}

expresion_booleana : expresion_booleana OP_BOOL expresion_booleana_base	{
										if (strcmp($2, OP_BOOL_AND) == 0)
										{
											if( ! atoi($1.value) || ! atoi($3.value))
											{
												$$ = createValueInfo(iota(0), BOOLEAN_T, $1.lexema);
											}
											else
											{
												$$ = createValueInfo(iota(1), BOOLEAN_T, $1.lexema);
											}
										}
										else
										{
											if( atoi($1.value) || atoi($3.value))
											{
												$$ = createValueInfo(iota(1), BOOLEAN_T, $1.lexema);
											}
											else
											{
												$$ = createValueInfo(iota(0), BOOLEAN_T, $1.lexema);
											}
											
										}
									}
		| NEGACION expresion_booleana_base	{
								int res = negateBoolean(atoi($2.value));
								$$ = createValueInfo(iota(res), BOOLEAN_T, $2.lexema);
							}
		| expresion_booleana_base	{ 
							$$ = createValueInfo($1.value, BOOLEAN_T, $1.lexema);
						}

expresion_booleana_base : lista_sumas OP_RELACIONAL lista_sumas {
									if(isNumberType($1.type) && isNumberType($3.type) && isSameType($1.type, $3.type))
									{
										int res = doRelationalOperation(atof($1.value), $2, atof($3.value));
										$$ = createValueInfo(iota(res), BOOLEAN_T, $1.lexema);
									}
									else
									{
										char * error = allocateSpaceForMessage();
										sprintf(error, "Cannot do comparation %s %s %s", $1.value, $2, $3.value);
										yyerror(error);
									}
								}
			| terminal_boolea	{
							if (isSameType($1.type, BOOLEAN_T))
							{
								$$ = createValueInfo($1.value, $1.type, $1.lexema);
							}
							else
							{
								char * error = allocateSpaceForMessage();
								sprintf(error, "%s is not valid for boolean expression", $1.value);
								yyerror(error);
							}
						}

terminal_boolea : BOOLEAN	{	
					$$ = createValueInfo(iota($1), BOOLEAN_T, NULL);
				}
		| PARENTESIS_ABIERTO expresion_booleana PARENTESIS_CERRADO	{
											if (isSameType($2.type, BOOLEAN_T))
											{
												$$ = createValueInfo($2.value, BOOLEAN_T, $2.lexema);
											}
											else
											{
												char * error = allocateSpaceForMessage();
												sprintf(error, "Cannot do operation with %s", $2.value);
												yyerror(error);
											}
										}

tensor : CORCHETE_ABIERTO lista_componentes CORCHETE_CERRADO	{
       									if (ampliar_vector_dims[$2.dim])
									{
										ampliar_vector_dims[$2.dim] = false;
									}
									$$.dim = $2.dim + 1;
									$$.type = $2.type;
									$$.elements = $2.elements;
									$$.num_elem = $2.num_elem;		
								}

lista_componentes : lista_componentes PUNTO_Y_COMA componente	{
									$$.dim = $1.dim;
									if (isSameType($1.type, INT32_T) && isSameType($3.type, INT32_T))
									{
										$$.type = INT32_T;
									}
									else
									{
										$$.type = FLOAT64_T;
									}
									$$.num_elem = $1.num_elem + $3.num_elem;
									$$.elements = realloc($1.elements, ($1.num_elem + $3.num_elem) * calculateSizeType($$.type));
									castTensorToVoidPointer($$.elements, $1.type, $1.num_elem, $3.elements, $3.type, $3.num_elem);	
									if (ampliar_vector_dims[$1.dim])
									{
										vector_dims_tensor[$1.dim] += 1;
									}
								}
		| componente	{
					$$.dim = $1.dim;
					$$.type = $1.type;
					$$.elements = $1.elements;
					$$.num_elem = $1.num_elem;
					if ($1.dim >= num_dims_tensor)
					{
						vector_dims_tensor = realloc(vector_dims_tensor, ++num_dims_tensor * 4);					
						vector_dims_tensor[$1.dim] = 1;
						ampliar_vector_dims = realloc(ampliar_vector_dims, num_dims_tensor);
						ampliar_vector_dims[$1.dim] = true;
					}
				}

componente : lista_valores	{
					$$.dim = $1.dim;
					$$.type = $1.type;
					$$.elements = $1.elements;
					$$.num_elem = $1.num_elem;
				}
	| tensor	{
				$$.dim = $1.dim;
				$$.type = $1.type;
				$$.elements = $1.elements;
				$$.num_elem = $1.num_elem;
			}

lista_valores : lista_valores COMA lista_sumas	{
							$$.dim = 0;
							if (isSameType($1.type, INT32_T) && isSameType($3.type, INT32_T))
							{
								$$.type = INT32_T;
							}
							else
							{
								$$.type = FLOAT64_T;
							}
							$$.elements = realloc($1.elements, ($1.num_elem + 1) * calculateSizeType($$.type));
							void * elem2 = malloc(calculateSizeType($3.type));
							castValueToVoidPointer(elem2, $3.value, $3.type);
							castTensorToVoidPointer($$.elements, $1.type, $1.num_elem, elem2, $3.type, 1);
							$$.num_elem = $1.num_elem + 1;
							if (ampliar_vector_dims[0])
							{
								vector_dims_tensor[0] += 1;
							}
						}
		| lista_sumas	{
					$$.dim = 0;
					$$.type = $1.type;
					$$.elements = malloc(calculateSizeType($1.type));
					castValueToVoidPointer($$.elements, $1.value, $1.type);
					$$.num_elem = 1;
					if (ampliar_vector_dims == NULL)
					{
						vector_dims_tensor = malloc(4);
						vector_dims_tensor[0] = 1;
						ampliar_vector_dims = malloc(1);
						ampliar_vector_dims[0] = true;
						num_dims_tensor++;
					}
				}


%%
