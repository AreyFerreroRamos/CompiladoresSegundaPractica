#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "tipus.h"

int init_analisi_lexic(char *);
int init_analisi_sintactic(char *);
int analisi_semantic();
int end_analisi_lexic();
int end_analisi_sintactic();
void debug(char *, char *);
void simpleDebug(char *);
int doOperationAritmeticP3(value_info v1, char *operand, value_info v2, value_info *finish_val);
int canTypeDoOperationP3(char *type);
int intOperations(int num1, int num2, char *operand, int *res);
float floatOperations(float num1, float num2, char *operand, float *res);
char *iota(int num);
char *fota(float num);

#endif
