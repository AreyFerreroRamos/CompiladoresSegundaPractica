%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>

  #include "tipus.h"
  #include "utils.h"
  #include "functions.h"

  #define YYLMAX 100

  extern FILE *yyout;
  extern int yylex();
  extern void yyerror(char *);

  	// Variables necesarias para la inicialización de un tensor.
  int *vector_dims_tensor;	// Vector con el número de elementos de cada dimensión del tensor.
  int num_dims_tensor = 0;	// Número de dimensiones del tensor.
  bool *ampliar_vector_dims; 	// Vector de booleanos para limitar la ampliación de memoria del vector de dimensiones a una sola ampliación por dimensión.

	//Variables para controlar el flujo de variables temporales en la symtab
  char **list_tmp_variables_symtab;
  int num_tmp_variable = 0;
  int sq = 0;
  int lengthCode = 0;
  char **c3a;

  	//Variable para controlar las funciones
  func_param_info functionInfo;

%}

%code requires
{
  #include "utils.h"
  #include "functions.h"
  #include "tipus.h"
}

%union
{
	struct
	{
		char *lexema;
		int lenght;
		int line;
	} ident;
	int enter;
	float real;
	char *cadena;
	bool boolea;
	value_info_base value_info_base;
	value_info operand;
	tensor_info tensor_info;
	tensor_ini_info tensor_ini_info;
	sym_value_type sym_value_type;
	func_param_info func_param_info;
	void *no_definit;
}

%token <no_definit> ASSIGN START VALUERETURN DIRECTRETURN END DOBLE_DOS_PUNTOS LLAVE_ABIERTA LLAVE_CERRADA
%token <enter> INTEGER
%token <real> FLOAT
%token <cadena> OP_ARIT_P1 OP_ARIT_P2 ASTERISCO OP_ARIT_P3 PARENTESIS_ABIERTO PARENTESIS_CERRADO DIV COMA CORCHETE_ABIERTO CORCHETE_CERRADO PUNTO_Y_COMA TYPE ID_PROC
%token <ident> ID 
%token <operand> ID_ARIT

%type <operand> expresion_aritmetica lista_sumas lista_productos terminal_aritmetico id_arit
%type <tensor_info> id lista_indices lista_indices_arit
%type <tensor_ini_info> tensor componente lista_componentes lista_valores
%type <cadena> op_arit_p1
%type <func_param_info> cabecera_procedimiento cabecera_funcion cabecera_accion lista_params
%type <value_info_base> param
%start programa


%%


programa : lista_de_procedimientos main | main

lista_de_procedimientos : lista_de_procedimientos procedimiento | procedimiento

procedimiento : cabecera_procedimiento lista_de_sentencias END
{
	if($1.returnType==NULL)
	{
	emet(INSTR_END,1,1);
	}
	else
	{
	emet(INSTR_END,1,0);
	}
	popSymtab();
	sym_value_type entry = createSymValueType($1.returnType,$1.numParams,0,NULL,$1.params,FUNC_T);

printf("LLEGO AQUI");
fflush(stdout);
	addOrUpdateEntry($1.funcName,entry);

}

cabecera_procedimiento : cabecera_funcion
			| cabecera_accion

main : lista_de_sentencias

lista_de_sentencias : lista_de_sentencias sentencia{createFuncParamInfo(NULL,0,"prueba",NULL);}| sentencia

sentencia : asignacion 
	| expresion_aritmetica 	{
					emet(INSTR_PUT, 1, $1.value);
				}
	| ID	{
			emet(INSTR_PUT, 1, $1.lexema);
		}
		| VALUERETURN expresion_aritmetica
		{
			emet(INSTR_RETURN,1,$2.value);
		}
		| DIRECTRETURN
		{
			emet(INSTR_RETURN,0);
		}

asignacion : ID ASSIGN expresion_aritmetica	{
							sym_value_type entry;
							entry = createSymValueType($3.type, calculateSizeType($3.type), 0, NULL, NULL, VAR_T);
							addOrUpdateEntry($1.lexema, entry);
							value_info v1 = createValueInfo($1.lexema, $3.type, VAR_T, generateEmptyValueInfoBase());
							emet(INSTR_COPY, 2, v1, $3);
						}
	| id ASSIGN expresion_aritmetica	{
							sym_value_type entry = getEntry($1.lexema);
							entry.type = $3.type;
							entry.size = getAcumElemDim(entry.elem_dims, entry.num_dim) * calculateSizeType($3.type);
							addOrUpdateEntry($1.lexema, entry);
							value_info v1 = createValueInfo($1.lexema, entry.type, TENS_T, $1.calcIndex);
							emet(INSTR_COPY, 2, v1, $3);
						}
	| ID ASSIGN tensor	{
					invertVector(vector_dims_tensor, $3.dim);
					sym_value_type entry = createSymValueType($3.type, calculateSizeType($3.type) * $3.num_elem, $3.dim, vector_dims_tensor, $3.elements, TENS_T);
					addOrUpdateEntry($1.lexema, entry);
					emetTensor($1.lexema, $3);
					vector_dims_tensor = NULL;
					ampliar_vector_dims = NULL;
					num_dims_tensor = 0;
				}

id : lista_indices CORCHETE_CERRADO	{
						$$ = $1;
					}
   
lista_indices : lista_indices COMA lista_sumas	{
							if (isSameType($3.type, INT32_T))
							{
								int dim = getDim($1.lexema, $1.index_dim);
								if (isSameType($1.calcIndex.valueInfoType, VAR_T) || isSameType($3.valueInfoType, VAR_T))
								{
									char *nameTmp = generateTmpId();
									sym_value_type entry = getEntry($1.lexema);
									value_info v1 = createValueInfo(nameTmp, entry.type, VAR_T, generateEmptyValueInfoBase());
									value_info v2 = createValueInfo($1.calcIndex.value, $1.calcIndex.type, $1.calcIndex.valueInfoType, generateEmptyValueInfoBase());
									value_info v3 = createValueInfo(itos(dim), INT32_T, LIT_T, generateEmptyValueInfoBase());
									emet(INSTR_MULI,3, v1, v2, v3);
									emet(INSTR_ADDI,3, v1, v1, $3);
									value_info_base calcIndex = createValueInfoBase(nameTmp, INT32_T, VAR_T);
									$$ = createTensorInfo($1.index_dim + 1, calcIndex, $1.lexema);
								}
								else
								{
									value_info_base calcIndex = createValueInfoBase(itos(atoi($1.calcIndex.value) * dim + atoi($3.value)), INT32_T, LIT_T);
									$$ = createTensorInfo($1.index_dim + 1, calcIndex, $1.lexema);
								}
							}
							else
							{
								yyerror(generateString("El indice -> %s es de tipo %s. El tipo debería ser INT32.", 2, $3.value, $3.type));
							}
						}
		| ID CORCHETE_ABIERTO lista_sumas	{
		      						if (isSameType($3.type, INT32_T))
								{
									value_info_base calcIndex;
									if (isSameType($3.valueInfoType, VAR_T))
									{
										calcIndex = createValueInfoBase($3.value, INT32_T, VAR_T);
									}
									else
									{
										calcIndex = createValueInfoBase($3.value, INT32_T, LIT_T);
									}
									$$ = createTensorInfo(1, calcIndex, $1.lexema);
								}
								else
								{
									yyerror(generateString("El indice -> %s es de tipo %s. El tipo debería ser INT32.", 2, $3.value, $3.type));
								}
		     					}

expresion_aritmetica : lista_sumas

lista_sumas : lista_sumas OP_ARIT_P2 lista_productos	{
								if (isNumberType($3.type))
								{	
									if (isSameType($1.valueInfoType, LIT_T) && isSameType($3.valueInfoType, LIT_T))
									{
										$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
										doAritmeticOperation($1, $2, $3, &$$);
									}
									else
									{
										//CONTROLAR OPERACIÓN VALIDA? (Ex: MODULO CON FLOATS)
										$$ = createValueInfo(generateTmpId(), getNewType($1.type, $3.type), VAR_T, generateEmptyValueInfoBase());
										classifyOperation($2, $$, $1, $3);
										//GUARDAR VARIABLE TMP EN SYMTAB?
									}
								}
								else
								{
									yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s", 1, $3.type));
								}
							}	
		| lista_productos	{
						if (isNumberType($1.type))
						{
							$$ = $1;
						}
						else
						{
							yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s", 1, $1.type));
						}
					}

lista_productos : lista_productos op_arit_p1 terminal_aritmetico	{
										if (isNumberType($3.type))
										{
											if (isSameType($1.valueInfoType, LIT_T) && isSameType($3.valueInfoType, LIT_T))
											{
												$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
												doAritmeticOperation($1, $2, $3, &$$);
											}
											else
											{
												//CONTROLAR OPERACIÓN VALIDA? (Ex: MODULO CON FLOATS)
												$$ = createValueInfo(generateTmpId(), getNewType($1.type, $3.type), VAR_T, generateEmptyValueInfoBase());
												classifyOperation($2, $$, $1, $3);
												//GUARDAR VARIABLE TMP EN SYMTAB?
											}
										}
										else
										{
											yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s", 1, $3.type));
										}
									}
		| terminal_aritmetico	{
						if (isNumberType($1.type))
						{
							$$ = $1;
						}
						else
						{
							yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s", 1, $1.type));
						}
					}

op_arit_p1: OP_ARIT_P1	{
				$$ = strdup($1);
			} 
	| ASTERISCO	{
				$$ = strdup($1);
			}

terminal_aritmetico : INTEGER	{
					$$ = createValueInfo(itos($1), INT32_T, LIT_T, generateEmptyValueInfoBase());
				}
	| FLOAT		{
				$$ = createValueInfo(ftos($1), FLOAT64_T, LIT_T, generateEmptyValueInfoBase());
			}
	| id_arit 	{
				$$ = $1;
			}
	| PARENTESIS_ABIERTO lista_sumas PARENTESIS_CERRADO	{
									if (isNumberType($2.type))
									{
										$$ = $2;
									}
									else
									{
										yyerror(generateString("No se pueden realizar operaciones aritméticas con el tipo %s", 1, $2.type));
									}
								}
	| DIV lista_sumas COMA lista_sumas PARENTESIS_CERRADO	{
								/*	if ((isNumberType($2.type)) && (isNumberType($4.type)))
									{
										$$.value = (char *) malloc(sizeof(char) * FLOAT_MAX_LENGTH_STR);
										if (!doAritmeticOperation($2, "/", $4, &$$))
										{
											yyerror("Ha habido algun problema realizando la operación");
										}
									}
									else
									{
										yyerror(generateString,"No se pueden realizar operaciones aritméticas con el tipo %s",1, $2.type);
									}*/
								}

id_arit : ID_ARIT	{
				$$ = $1;
			}
	| lista_indices_arit CORCHETE_CERRADO	{
								sym_value_type entry = getEntry($1.lexema);
								char *nameTmp = generateTmpId();
								value_info v1 = createValueInfo(nameTmp, entry.type, VAR_T, generateEmptyValueInfoBase());
								value_info v2 = createValueInfo($1.lexema, entry.type, TENS_T, $1.calcIndex);
								emet(INSTR_COPY,2, v1, v2);
								$$ = createValueInfo(nameTmp, entry.type, VAR_T, generateEmptyValueInfoBase());
						}

lista_indices_arit : lista_indices_arit COMA lista_sumas	{
									if (isSameType($3.type, INT32_T))
									{
										int dim = getDim($1.lexema, $1.index_dim);
										if (isSameType($1.calcIndex.valueInfoType, VAR_T) || isSameType($3.valueInfoType, VAR_T))
										{
											char *nameTmp = generateTmpId();
											sym_value_type entry = getEntry($1.lexema);
											value_info v1 = createValueInfo(nameTmp, entry.type, VAR_T, generateEmptyValueInfoBase());
											value_info v2 = createValueInfo($1.calcIndex.value, $1.calcIndex.type, $1.calcIndex.valueInfoType, generateEmptyValueInfoBase());
											value_info v3 = createValueInfo(itos(dim), INT32_T, LIT_T, generateEmptyValueInfoBase());
											emet(INSTR_MULI,3, v1, v2, v3);
											emet(INSTR_ADDI,3, v1, v1, $3);
											value_info_base calcIndex = createValueInfoBase(nameTmp, INT32_T, VAR_T);
											$$ = createTensorInfo($1.index_dim + 1, calcIndex, $1.lexema);
										}
										else
										{
											value_info_base calcIndex = createValueInfoBase(itos(atoi($1.calcIndex.value) * dim + atoi($3.value)), INT32_T, LIT_T);
											$$ = createTensorInfo($1.index_dim + 1, calcIndex, $1.lexema);
										}
									}
									else
									{
										yyerror(generateString("El indice -> %s es de tipo %s. El tipo debería ser INT32.", 2, $3.value, $3.type));
									}
								}
		| ID_ARIT CORCHETE_ABIERTO lista_sumas	{
	   							if (isSameType($3.type, INT32_T)) 
								{
									value_info_base calcIndex;
									if (isSameType($3.valueInfoType, VAR_T))
									{
										calcIndex = createValueInfoBase($3.value, INT32_T, VAR_T);
									}
									else
									{
										calcIndex = createValueInfoBase($3.value, INT32_T, LIT_T);
									}
									$$ = createTensorInfo(1, calcIndex, $1.value);
								}
								else
								{
									yyerror(generateString("El indice -> %s es de tipo %s. El tipo debería ser INT32.", 2, $3.value, $3.type));
								}
							}

tensor : CORCHETE_ABIERTO lista_componentes CORCHETE_CERRADO	{
       									if (ampliar_vector_dims[$2.dim])
       									{
       										ampliar_vector_dims[$2.dim] = false;
       									}
       					                		$2.dim++;
       					                		$$ = $2;
								}

lista_componentes : lista_componentes PUNTO_Y_COMA componente	{
									$$ = createTensorIniInfo($1.dim, getNewType($1.type, $3.type), joinElementsVectors($1.elements, $3.elements, $1.num_elem, $3.num_elem), $1.num_elem + $3.num_elem);
									if (ampliar_vector_dims[$1.dim])
									{
										vector_dims_tensor[$1.dim] += 1;
									}
								}
		| componente	{
					$$ = $1;
					if ($1.dim >= num_dims_tensor)
					{
						vector_dims_tensor = realloc(vector_dims_tensor, ++num_dims_tensor * 4);					
						vector_dims_tensor[$1.dim] = 1;
						ampliar_vector_dims = realloc(ampliar_vector_dims, num_dims_tensor);
						ampliar_vector_dims[$1.dim] = true;
					}
				}

componente : tensor	{
				$$ = $1;
			}
	| lista_valores	{
				$$ = $1;
			}

lista_valores : lista_valores COMA lista_sumas	{
							$$ = createTensorIniInfo(0, getNewType($1.type, $3.type), joinElementsVectors($1.elements, castValueInfoToValueInfoBase($3), $1.num_elem, 1), $1.num_elem + 1);
							if (ampliar_vector_dims[0])
							{
								vector_dims_tensor[0] += 1;
							}
						}
		| lista_sumas	{
					$$ = createTensorIniInfo(0, $1.type, castValueInfoToValueInfoBase($1), 1);
					if (ampliar_vector_dims == NULL)
					{
						ampliar_vector_dims = malloc(1);
						ampliar_vector_dims[0] = true;
						vector_dims_tensor = malloc(4);
						vector_dims_tensor[0] = 1;
						num_dims_tensor++;
					}
				}

cabecera_funcion : cabecera_accion DOBLE_DOS_PUNTOS TYPE
{
	$1.returnType = strdup($3);
	$$ = $1;
}

cabecera_accion : START ID_PROC PARENTESIS_ABIERTO lista_params PARENTESIS_CERRADO
{
	emet(INSTR_START,1,$2);
	$4.funcName = strdup($2);
	$$=$4;
}

lista_params : lista_params COMA param
		{
		addValueInfoBase($1.params,$1.numParams,$3);
		$1.numParams++;
		$$=$1;
		}
		| param
		{
		$$ = createFuncParamInfo(NULL,0,NULL,NULL);
		addValueInfoBase($$.params,$$.numParams,$1);
		$$.numParams++;
		}

param : ID DOBLE_DOS_PUNTOS TYPE
	{
		value_info_base v = createValueInfoBase($1.lexema,$3,VAR_T);
		sym_value_type entry = castValueInfoBaseToSymValueType(v);
		addOrUpdateEntry(v.value,entry);
		$$ = v;
	}
	| ID DOBLE_DOS_PUNTOS TYPE LLAVE_ABIERTA TYPE LLAVE_CERRADA
	{
		if(isSameType($3,TENSOR_T))
		{
			value_info_base v = createValueInfoBase($1.lexema,$5,TENS_T);
			sym_value_type entry = castValueInfoBaseToSymValueType(v);
			addOrUpdateEntry(v.value,entry);
			$$ = v;
		}else{
			yyerror("El tipo debería ser un tensor");
		}
	}





%%
