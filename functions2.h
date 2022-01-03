#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "tipus.h"
#include "symtab.h"

// FUNCIONES BASE PARA EJECUCIÓN DEL COMPILADOR

int init_analisi_lexic(char *);
int init_analisi_sintactic(char *);
int analisi_semantic();
int end_analisi_lexic();
int end_analisi_sintactic();
void yyerror(char *);

// FUNCIONES DE UTILIDAD

/**
*
**/
char *generateTmpId();

/**
*
**/
void emet(char * type, value_info v1, value_info v2, value_info v3);
