/*
Projekt do předmětů IFJ a IAL (ZS 2021)
Tým 031, varianta I

Autoři:
xploci01: Peter Pločica
xsouce15: Tomáš Souček
xcolog00: Adam Cologna
*/

#include "symtable.h"

extern symtable_t st;
void Expression();

char* copyString(char* string);

void typestack_init(int size);
void typestack_push(char type);
char typestack_pop();
int typestack_items();
