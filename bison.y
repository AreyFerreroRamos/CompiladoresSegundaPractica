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
	int sq = 0;	
	int lengthCode = 0;
	char **c3a;
%}

%code requires {
  #include "functions2.h"
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
%token <cadena> OP_ARIT_P1 OP_ARIT_P2 ASTERISCO OP_ARIT_P3 PARENTESIS_ABIERTO PARENTESIS_CERRADO DIV COMA CORCHETE_ABIERTO CORCHETE_CERRADO PUNTO_Y_COMA
%token <ident> ID 
%token <operand> ID_ARIT

%type <operand> expresion_aritmetica lista_sumas lista_productos lista_potencias terminal_aritmetico id_arit
%type <tensor_info> id lista_indices lista_indices_arit
%type <tensor_ini_info> tensor componente lista_componentes lista_valores
%type <cadena> op_arit_p2

%start programa


%%


programa : lista_de_sentencias

lista_de_sentencias : lista_de_sentencias sentencia | sentencia

sentencia : asignacion 
	| expresion_aritmetica 	{
					//fprintf(yyout, "El resultado es %s\n", $1.value);
				}
	| ID	{
			/*sym_value_type entry;
			int response = sym_lookup($1.lexema, &entry);
			if (response == SYMTAB_OK) 
			{
				if (entry.num_dim > 0){ printTensor($1.lexema, entry, 1);}
				else{ fprintf(yyout, "ID: %s val:%s\n", $1.lexema, (char *) entry.value);}
			}*/
		}

asignacion : ID ASSIGN expresion_aritmetica	{
							sym_value_type entry;
							int size=0;
							if (isSameType($3.valueInfoType,LIT_T) || isSameType($3.valueInfoType,VAR_T)) {
								size = isSameType($3.type,INT32_T) ? calculateSizeType(INT32_T) : calculateSizeType(FLOAT64_T);
								entry = createSymValueType($3.type,size,0,NULL,NULL,VAR_T);
								int message = sym_enter($1.lexema, &entry);
								if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE){ yyerror("Error al guardar en symtab");}
								value_info v1 = createValueInfo(NULL,$3.type,$1.lexema,VAR_T,-1);
								value_info v3;
								emet(INSTR_COPY,v1,$3,v3);
							}
							/*else if(isSameType($3.valueInfoType,TENS_T))
							{
								int response = sym_lookup($3.lexema, &entry);
								if (response == SYMTAB_OK)
								{
									response = sym_enter($1.lexema, &entry);
									if (response != SYMTAB_OK && response != SYMTAB_DUPLICATE){ yyerror("Error al guardar en symtab");}
									printTensor($1.lexema, entry, 1);
									clearTmpTensorId();
								}
							}*/
						}
	| id ASSIGN expresion_aritmetica	{	
							/*if ($3.value != NULL)
							{
								sym_value_type entry;
								int response = sym_lookup($1.lexema, &entry);
								if (response == SYMTAB_OK)
								{
									if (isSameType(entry.type, INT32_T))
									{	
										if (isSameType($3.type, INT32_T)){ ((int *) entry.elements)[$1.calcIndex] = atoi($3.value);}
										else{ ((int *) entry.elements)[$1.calcIndex] = (int) atof($3.value);}
									}
									else if (isSameType(entry.type, FLOAT64_T))
									{
										if (isSameType($3.type,INT32_T)){ ((float *) entry.elements)[$1.calcIndex] = atoi($3.value);}
										else{ ((float *) entry.elements)[$1.calcIndex] = atof($3.value);}
									}
									response = sym_enter($1.lexema, &entry);

									if (response != SYMTAB_OK && response != SYMTAB_DUPLICATE){ yyerror("Error al guardar en symtab");}

									fprintf(yyout, "ID: %s pren per valor: %s a la posicio: %i\n", $1.lexema, (char *) $3.value, $1.calcIndex);
								}
								else{ yyerror("Error al cargar variable de la symtab");}
							}
							else{ yyerror("No se puede asignar un tensor a un indice de un tensor");}*/
						}
	| ID ASSIGN tensor	{	
					/*sym_value_type entry;
					entry.type = $3.type;
					entry.value = NULL;
					entry.size = calculateSizeType($3.type) * $3.num_elem;
					entry.num_dim = $3.dim;
					invertVector(vector_dims_tensor, $3.dim);
					entry.elem_dims = vector_dims_tensor;
					entry.elements = $3.elements;
					int message = sym_enter($1.lexema, &entry);

					if (message != SYMTAB_OK && message != SYMTAB_DUPLICATE){ yyerror("Error al guardar en symtab.");}

					printTensor($1.lexema, entry, 1);
					vector_dims_tensor = NULL;
					ampliar_vector_dims = NULL;
					num_dims_tensor = 0;*/
				}

id : lista_indices CORCHETE_CERRADO	{
						//$$ = createTensorInfo($1.index_dim, $1.calcIndex, $1.lexema);
					}
   
lista_indices : lista_indices COMA lista_sumas	{
							/*if (isSameType($3.type, INT32_T)) 
							{
								int dim = getDim($1.lexema, $1.index_dim);
								if (dim != -1){ $$ = createTensorInfo($1.index_dim + 1, $1.calcIndex * dim + atoi($3.value), $1.lexema);}
								else{ yyerror(generateString("Mal acceso a la variable %s",1,$1.lexema);}
							}
							else
							{
								if(isSameType($3.valueInfoType,LIT_T)){ yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.value,$3.type));}
									else{ yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.lexema,$3.type));}
							}*/
						}
		| ID CORCHETE_ABIERTO lista_sumas	{
		      					/*	if (isSameType($3.type, INT32_T)){ $$ = createTensorInfo(1, atoi($3.value), $1.lexema);}
								else
								{
									if(isSameType($3.valueInfoType,LIT_T)){ yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.value,$3.type));}
									else{ yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.lexema,$3.type));}
								}*/
		     					}

expresion_aritmetica : lista_sumas

lista_sumas : lista_sumas OP_ARIT_P3 lista_productos	{
	    							
								if (isNumberType($3.type))
								{	
									$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
									if (isSameType($1.valueInfoType, _LIT_T)) {
										
										if (!doAritmeticOperation($1, $2, $3, &$$)) { 
											yyerror("Algún problema durante la operación.");
										}
										
								}
								else{ yyerror(generateString,"No se pueden realizar operaciones aritméticas con el tipo %s",1, $3.type); }
							}	
		| lista_productos	{ 	
						if (isNumberType($1.type)){ $$ = $1;}
						else{ yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s",1, $1.type));}
					}

lista_productos : lista_productos op_arit_p2 lista_potencias 	{
								/*	if (isNumberType($3.type))
									{
										int response = -4;
										sym_value_type tmp;
										if (strcmp($2, OP_ARIT_MULT) == 0){ response = doTensorProductInit($1.lexema, $3.lexema, &tmp);}
										if (response == 0)
										{
											int res = doTensorProductTensor($1.lexema, $3.lexema, &tmp);

											if (res != 0){ yyerror("Alguna variable no se ha encontrado en la symtab");}
											else{ saveTmpTensorInSymTab(&$$, $1.type, $3.type, tmp);}
										}
										else if (response == -1){ yyerror("Los indices de los tensores no son compatibles y no se puede realizar el producto");}
										else if (response == -2){ yyerror("No esta permitido multiplicar tensores de dimensión superior a 2");}
										else if (response == -3)
										{
											int res;

											if ($1.value != NULL){ res = doNumberProductTensor($1.value, $1.type, $3.lexema, &tmp);}
											else{ res = doNumberProductTensor($3.value, $3.type, $1.lexema, &tmp);}

											if (res != 0){ yyerror("Alguna variable no se ha encontrado en la symtab.");}
											else{ saveTmpTensorInSymTab(&$$, $1.type, $3.type, tmp);}
										}
										else if (response == -4)
										{
											if ($1.lexema == NULL && $3.lexema == NULL)
											{
												$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
												if (!doAritmeticOperation($1, $2, $3, &$$)){ yyerror("Algún problema durante la operación.");}
											}
											else{ yyerror("Los tensores no admiten la división.");}
										}
									}
									else{ yyerror(generateString,"No se pueden realizar operaciones aritméticas con el tipo %s",1, $3.type); }*/
								}
		| lista_potencias	{
						if (isNumberType($1.type)){ $$ = $1;}
						else{ yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s",1, $1.type));}
					}

op_arit_p2: OP_ARIT_P2	{
				//$$ = strdup($1);
			} 
	| ASTERISCO	{
				//$$ = strdup($1);
			}

lista_potencias : lista_potencias OP_ARIT_P1 terminal_aritmetico {
									/*if (isNumberType($3.type))
									{
										$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
										if (!doAritmeticOperation($1, $2, $3, &$$)){ yyerror("Ha habido algun problema realizando la operación");}
									}
									else{ yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s",1, $3.type)); }*/
								}
		| terminal_aritmetico	{
						if (isNumberType($1.type)){ $$ = $1;}
						else{ yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s",1, $1.type));}
					}

terminal_aritmetico : INTEGER	{
					$$ = createValueInfo(itos($1), INT32_T, NULL,LIT_T,-1);
				}
	| FLOAT		{
				$$ = createValueInfo(ftos($1), FLOAT64_T, NULL,LIT_T,-1);
			}
	| id_arit 	{
				$$ = $1;
			}
	| PARENTESIS_ABIERTO lista_sumas PARENTESIS_CERRADO	{
									/*if (isNumberType($2.type)){ $$ = createValueInfo($2.value, $2.type, $2.lexema);}
									else{ yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s",1, $2.type));}	*/
								}
	| DIV lista_sumas COMA lista_sumas PARENTESIS_CERRADO	{
								/*	if ((isNumberType($2.type)) && (isNumberType($4.type)))
									{
										$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
										if (!doAritmeticOperation($2, "/", $4, &$$)){ yyerror("Ha habido algun problema realizando la operación");}
									}
									else{ yyerror(generateString,"No se pueden realizar operaciones aritméticas con el tipo %s",1, $2.type);}*/
								}

id_arit : ID_ARIT	{ 
			//	$$ = $1; 
			}	
	| lista_indices_arit CORCHETE_CERRADO	{
						/*	sym_value_type res;
							sym_lookup($1.lexema, &res);

							if (isSameType(res.type, INT32_T)){ $$.value = iota(((int *) res.elements)[$1.calcIndex]);}
							else{ $$.value = fota(((float *) res.elements)[$1.calcIndex]);}

							$$.type = res.type;
							$$.lexema = NULL;*/
						}

lista_indices_arit : lista_indices_arit COMA lista_sumas	{
							/*		if (isSameType($3.type, INT32_T)) 
									{
										int dim = getDim($1.lexema, $1.index_dim);

										if (dim != -1){ $$ = createTensorInfo($1.index_dim + 1, $1.calcIndex * dim + atoi($3.value), $1.lexema);}
										else{yyerror(generateString("Mal acceso a la variable %s",1,$1.lexema);}
									}
									else
									{
										if(isSameType($3.valueInfoType,LIT_T)){ yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.value,$3.type));}
										else yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.lexema,$3.type));}
									}			*/		
								}
		| ID_ARIT CORCHETE_ABIERTO lista_sumas	{
	   					/*		if (isSameType($3.type, INT32_T)) 
								{
									$$ = createTensorInfo(1, atoi($3.value), $1.lexema);
								}
								else
								{
									if(isSameType($3.valueInfoType,LIT_T)){ yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.value,$3.type));}
									else{ yyerror(generateString("El indice -> %s es %s,debería ser INT32",2,$3.lexema,$3.type));}
								}*/
							}

tensor : CORCHETE_ABIERTO lista_componentes CORCHETE_CERRADO	{
       					/*				if (ampliar_vector_dims[$2.dim]){ampliar_vector_dims[$2.dim] = false;}
										$$.dim = $2.dim + 1;
										$$.type = $2.type;
										$$.elements = $2.elements;
										$$.num_elem = $2.num_elem;		*/
								}

lista_componentes : lista_componentes PUNTO_Y_COMA componente	{
						/*			$$.dim = $1.dim;

									if (isSameType($1.type, INT32_T) && isSameType($3.type, INT32_T)){ $$.type = INT32_T;}
									else{$$.type = FLOAT64_T;}

									$$.num_elem = $1.num_elem + $3.num_elem;
									$$.elements = realloc($1.elements, ($1.num_elem + $3.num_elem) * calculateSizeType($$.type));
									castTensorToVoidPointer($$.elements, $1.type, $1.num_elem, $3.elements, $3.type, $3.num_elem);	

									if (ampliar_vector_dims[$1.dim]){ vector_dims_tensor[$1.dim] += 1;}*/
								}
		| componente	{
			/*		$$.dim = $1.dim;
					$$.type = $1.type;
					$$.elements = $1.elements;
					$$.num_elem = $1.num_elem;
					if ($1.dim >= num_dims_tensor)
					{
						vector_dims_tensor = realloc(vector_dims_tensor, ++num_dims_tensor * 4);					
						vector_dims_tensor[$1.dim] = 1;
						ampliar_vector_dims = realloc(ampliar_vector_dims, num_dims_tensor);
						ampliar_vector_dims[$1.dim] = true;
					}*/
				}

componente : lista_valores	{
			/*		$$.dim = $1.dim;
					$$.type = $1.type;
					$$.elements = $1.elements;
					$$.num_elem = $1.num_elem;*/
				}
	| tensor	{
		/*		$$.dim = $1.dim;
				$$.type = $1.type;
				$$.elements = $1.elements;
				$$.num_elem = $1.num_elem;*/
			}

lista_valores : lista_valores COMA lista_sumas	{
						/*	$$.dim = 0;

							if (isSameType($1.type, INT32_T) && isSameType($3.type, INT32_T)){ $$.type = INT32_T;}
							else{ $$.type = FLOAT64_T;}

							$$.elements = realloc($1.elements, ($1.num_elem + 1) * calculateSizeType($$.type));
							void * elem2 = malloc(calculateSizeType($3.type));
							castValueToVoidPointer(elem2, $3.value, $3.type);
							castTensorToVoidPointer($$.elements, $1.type, $1.num_elem, elem2, $3.type, 1);
							$$.num_elem = $1.num_elem + 1;

							if (ampliar_vector_dims[0]){ vector_dims_tensor[0] += 1; }*/
						}
		| lista_sumas	{
				/*	$$.dim = 0;
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
					}*/
				}


%%
