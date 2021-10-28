#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern int init_analisi_lexic(char *);
extern int init_analisi_sintactic(char *);
extern int analisi_semantic();
extern int end_analisi_lexic();
extern int end_analisi_sintactic();
extern void debug(char *, char *);

#endif