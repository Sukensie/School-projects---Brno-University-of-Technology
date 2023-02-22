/*
Projekt do předmětů IFJ a IAL (ZS 2021)
Tým 031, varianta I

Autoři:
xploci01: Peter Pločica
xsouce15: Tomáš Souček
xcolog00: Adam Cologna
*/


#include <stdio.h>
#include <stdlib.h>

#define LEX_ERROR 1
#define SYNTAX_ERROR 2
#define PROGRAM_SEM_ERROR 3
#define ASSIGNMENT_SEM_ERROR 4
#define PARAM_AMOUNT_SEM_ERROR 5
#define SEM_COMPATIBILITY_ERROR 6
#define OTHER_SEM_ERROR 7
#define NIL_ERROR 8
#define DIV_BY_ZERO_ERROR 9
#define INTERNAL_ERROR 99

void call_error(int code)
{
    fprintf(stderr, "\nProgram exited with code %d\n", code);
    exit(code);
}
