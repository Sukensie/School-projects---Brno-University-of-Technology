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
#include <string.h>
#include <ctype.h>

#include "code.h"
#include "string_lib.c"
#include "errors.h"

// Generovanie pociatocneho kodu - hlavicka a pomocne globalne premenne
void genMain_start(){
    printf(".IFJcode21\nDEFVAR GF@$1\nDEFVAR GF@$2\nDEFVAR GF@$3\nDEFVAR GF@$t\nDEFVAR GF@$p\nDEFVAR GF@$d\n");
}

// Generovanie kodu pre cele cislo
void genNumberInt(char* atr){
    printf("PUSHS int@%s\n", atr);
}

// Generovanie kodu pre realne cislo
void genNumberFloat(char* atr){
    double val = atof(atr);
    printf("PUSHS float@%a\n", val);
}

// Generovanie kodu pre retazec
void genString(char* atr){
    printf("PUSHS string@%s\n", atr);
}

// Generovanie kodu pre volanie funkcie s menom atr
void genFncCall(char* atr){
    printf("CALL %s\n", atr);
}

// Generovanie kodu pre zaciatok funkcie
char* vardef = NULL; // Na uchovanie kodu lokalnych premennych 
void genFncStart(char* atr, int labnum){
    vardef = NULL;
    printf("JUMP $m%d\nLABEL %s\nCREATEFRAME\nPUSHFRAME\nJUMP %s$vardef\nLABEL %s$start\n", labnum, atr, atr, atr);
}

// Generovanie kodu pre ukoncenie funkcie, doplnenie kodu lokal. premennych na vystup
void genFncEnd(char* name, int labnum, int cnt){
    for(int i = 0; i < cnt; i++){
        printf("PUSHS nil@nil\n");
    }
    printf("POPFRAME\nRETURN\nLABEL %s$vardef\n", name);
    if(vardef != NULL){
        printf("%s",  vardef);
        free(vardef);
        vardef = NULL;
    }
    printf("JUMP %s$start\nLABEL $m%d\n", name, labnum);

}

// Generovanie kodu na uvolnenie prebytocnych hodnot
void genDiscard(int cnt){
    for(int i = 0; i < cnt; i++){
        printf("POPS GF@$1\n");
    }
}

// Generovanie kodu pre pouzitie premennej
void genVarUse(char* name, int blkid){
    printf("PUSHS LF@%s$%d\n", name, blkid);
}

// Generovanie kodu pre konstantu nil
void genNil(){
    printf("PUSHS nil@nil\n");
}

// Generovanie kodu pre definiciu premennej
void genVarDef(char* name, int blkid){
    char* tmp = (char*)malloc(strlen(name)+20);
    if(tmp == NULL)
        call_error(INTERNAL_ERROR);
    sprintf(tmp,"DEFVAR LF@%s$%d\n", name, blkid);
    if(vardef == NULL){
        vardef = tmp;
    }
    else{
        vardef = (char*)realloc(vardef, strlen(vardef)+strlen(tmp)+1);
        if(vardef == NULL)
            call_error(INTERNAL_ERROR);
        strcat(vardef, tmp);
        free(tmp);
    }
}

// Generovanie kodu pre priradenie hodnoty do premennej "name" v bloku 'blkid'
void genVarAssign(char* name, int blkid){
    printf("POPS LF@%s$%d\n", name, blkid);
}

// Generovanie kodu pre priradenie hodnoty parametru funkcie "name"
void genParamAsign(char* name){
    char* tmp = (char*)malloc(strlen(name)+20);
    if(tmp == NULL)
        call_error(INTERNAL_ERROR);
    sprintf(tmp,"DEFVAR LF@%s$1\n", name);
    if(vardef == NULL){
        vardef = tmp;
    }
    else{
        vardef = (char*)realloc(vardef, strlen(vardef)+strlen(tmp)+1);
        if(vardef == NULL)
            call_error(INTERNAL_ERROR);
        strcat(vardef, tmp);
        free(tmp);
    }
    printf("POPS LF@%s$1\n", name);
}

// Generovanie kodu pre ukoncenie programu, obsahuje aj pomocne ukoncenie s chybou '8' a '9'
void genExit(){
    printf("EXIT int@0\nLABEL exit$8\nEXIT int@8\n\nLABEL exit$9\nEXIT int@9\n");
}

// Generovanie kodu pre vypis argumentu vo funkcii "write"
void genCallWrite(){
    printf("CALL write\n");
}

// Generovanie kodu funkciu "write", kde osetruje vypis nil
void genWrite(){
    printf("LABEL write\nPOPS GF@$1\nJUMPIFEQ write$nil GF@$1 nil@nil\nWRITE GF@$1\nRETURN\nLABEL write$nil\nWRITE string@nil\nRETURN\n");
}

// Generovanie kodu vstavanej funkcie "readi"
void genReadi(){
    printf("LABEL readi\nREAD GF@$1 int\nPUSHS GF@$1\nRETURN\n");
}

// Generovanie kodu vstavanej funkcie "reads"
void genReads(){
    printf("LABEL reads\nREAD GF@$1 string\nPUSHS GF@$1\nRETURN\n");
}

// Generovanie kodu vstavanej funkcie "readn"
void genReadn(){
    printf("LABEL readn\nREAD GF@$1 float\nPUSHS GF@$1\nRETURN\n");
}

// Generovanie kodu vstavanej funkcie "tointeger"
void genTointeger(){
    printf("LABEL tointeger\nFLOAT2INTS\nRETURN\n");
}

// Generovanie kodu vstavanej funkcie "substr"
void genSubstr(){
    printf("LABEL substr\nPOPS GF@$1\nPOPS GF@$2\nPOPS GF@$3\nJUMPIFEQ substr$3 GF@$1 nil@nil\nJUMPIFEQ substr$3 GF@$2 nil@nil\nJUMPIFEQ substr$3 GF@$3 nil@nil\n");
    printf("LT GF@$p GF@$2 int@1\nJUMPIFEQ substr$2 GF@$p bool@true\nSTRLEN GF@$d GF@$3\nGT GF@$p GF@$2 GF@$d\nJUMPIFEQ substr$2 GF@$p bool@true\n");
    printf("LT GF@$p GF@$1 int@1\nJUMPIFEQ substr$2 GF@$p bool@true\nGT GF@$p GF@$1 GF@$d\nJUMPIFEQ substr$2 GF@$p bool@true\nGT GF@$p GF@$2 GF@$1\nJUMPIFEQ substr$2 GF@$p bool@true\n");
    printf("MOVE GF@$p string@\nSUB GF@$2 GF@$2 int@1\nSUB GF@$1 GF@$1 int@1\nLABEL substr$0\nGETCHAR GF@$d GF@$3 GF@$2\nCONCAT GF@$p GF@$p GF@$d\nADD GF@$2 GF@$2 int@1\nGT GF@$d GF@$2 GF@$1\nJUMPIFEQ substr$1 GF@$d bool@true\n");
    printf("JUMP substr$0\nLABEL substr$1\nPUSHS GF@$p\nRETURN\nLABEL substr$2\nPUSHS nil@nil\nRETURN\nLABEL substr$3\nEXIT int@8\n");
}

// Generovanie kodu vstavanej funkcie "ord"
void genOrd(){
    printf("LABEL ord\nPOPS GF@$1\nPOPS GF@$2\nJUMPIFEQ ord$2 GF@$1 nil@nil\nJUMPIFEQ ord$2 GF@$2 nil@nil\n");
    printf("STRLEN GF@$d GF@$2\nLT GF@$p GF@$1 int@1\nJUMPIFEQ ord$1 GF@$p bool@true\nGT GF@$p GF@$1 GF@$d\nJUMPIFEQ ord$1 GF@$p bool@true\n");
    printf("PUSHS GF@$2\nPUSHS GF@$1\nPUSHS int@1\nSUBS\nSTRI2INTS\nRETURN\nLABEL ord$1\nPUSHS nil@nil\nRETURN\nLABEL ord$2\nEXIT int@8\n");
}

// Generovanie kodu vstavanej funkcie "chr"
void genChr(){
    printf("LABEL chr\nPOPS GF@$1\nJUMPIFEQ  chr$2 GF@$1 nil@nil\nLT GF@$p GF@$1 int@0\nJUMPIFEQ chr$1 GF@$p bool@true\n");
    printf("GT GF@$p GF@$1 int@255\nJUMPIFEQ chr$1 GF@$p bool@true\nPUSHS GF@$1\nINT2CHARS\nRETURN\n");
    printf("LABEL chr$1\nPUSHS nil@nil\nRETURN\nLABEL chr$2\nEXIT int@8\n");
}

// Generovanie kodu pre zaciatok "while"
void genWhileStart(int lab){
    printf("LABEL $while1$%d\n", lab);
}

// Generovanie kodu pre vyhodnotenie podmienky "while"
void genWhileExpr(int lab){
    printf("POPS GF@$1\nJUMPIFEQ  $while3$%d GF@$1 nil@nil\n", lab);
    printf("TYPE GF@$t GF@$1\nJUMPIFNEQ $while2$%d GF@$t string@bool\nJUMPIFEQ $while3$%d bool@false GF@$1\nLABEL $while2$%d\n", lab, lab, lab);
}

// Generovanie kodu pre ukoncenie "while"
void genWhileEnd(int lab){
    printf("JUMP $while1$%d\nLABEL $while3$%d\n", lab, lab);
}

// Generovanie kodu pre vyhodnotenie podmienky "if"
void genIfExpr(int lab){
    printf("POPS GF@$1\nJUMPIFEQ  $if2$%d GF@$1 nil@nil\n", lab);
    printf("TYPE GF@$t GF@$1\nJUMPIFNEQ $if1$%d GF@$t string@bool\nJUMPIFEQ $if2$%d bool@false GF@$1\nLABEL $if1$%d\n", lab, lab, lab);
}

// Generovanie kodu pre "else"
void genIfThenElse(int lab){
    printf("JUMP $if3$%d\nLABEL $if2$%d\n", lab, lab);
}

// Generovanie kodu pre ukoncenie "if"
void genIfEnd(int lab){
    printf("LABEL $if3$%d\n", lab);
}

// Generovanie kodu pre operator '+'
void genPlus(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == t2)
        printf("ADDS\n");
    else if(t1 == 'F'){ //vrchny treba prekonvertovat
        printf("INT2FLOATS\nADDS\n");
    }
    else{ //spodny treba prekonvertovat
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\nADDS\n");
    }
}

// Generovanie kodu pre operator '-'
void genMinus(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == t2)
        printf("SUBS\n");
    else if(t1 == 'F'){ //vrchny treba prekonvertovat
        printf("INT2FLOATS\nSUBS\n");
    }
    else{ //spodny treba prekonvertovat
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\nSUBS\n");
    }
}

// Generovanie kodu pre operator '#'
void genLength(){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nJUMPIFEQ exit$8 GF@$1 nil@nil\nPUSHS GF@$1\n");
    printf("POPS GF@$1\nSTRLEN GF@$d GF@$1\nPUSHS GF@$d\n");
}

// Generovanie kodu pre operator '*'
void genMultiply(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == t2)
        printf("MULS\n");
    else if(t1 == 'F'){ //vrchny treba prekonvertovat
        printf("INT2FLOATS\nMULS\n");
    }
    else{ //spodny treba prekonvertovat
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\nMULS\n");
    }
}

// Generovanie kodu pre operator '/'
void genDivide(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == t2){
        if(t2 == 'I')
            printf("INT2FLOATS\nPOPS GF@$1\nINT2FLOATS\nJUMPIFEQ exit$9 GF@$1 float@0x0.0000p+0\nPUSHS GF@$1\nDIVS\n");
        else
            printf("POPS GF@$1\nJUMPIFEQ exit$9 GF@$1 float@0x0.0000p+0\nPUSHS GF@$1\nDIVS\n");
    }
    else if(t1 == 'F'){ //vrchny treba prekonvertovat
        printf("INT2FLOATS\nPOPS GF@$1\nJUMPIFEQ exit$9 GF@$1 float@0x0.0000p+0\nPUSHS GF@$1\nDIVS\n");
    }
    else{ //spodny treba prekonvertovat
        printf("POPS GF@$1\nJUMPIFEQ exit$9 GF@$1 float@0x0.0000p+0\nINT2FLOATS\nPUSHS GF@$1\nDIVS\n");
    }
}

// Generovanie kodu pre operator '//'
void genIntDivide(){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    printf("POPS GF@$1\nJUMPIFEQ exit$9 GF@$1 int@0\nPUSHS GF@$1\nIDIVS\n");
}

// Generovanie kodu pre operator '..'
void genConcatenate(){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    printf("POPS GF@$1\nPOPS GF@$2\nCONCAT GF@$p GF@$2 GF@$1\nPUSHS GF@$p\n");
}

// Generovanie kodu pre operator '~='
static int labneq = 1;
void genNotEqual(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nPUSHS GF@$2\nPUSHS GF@$1\nJUMPIFEQ neq$%d GF@$1 nil@nil\nJUMPIFEQ neq$%d GF@$2 nil@nil\n", labneq, labneq);
    //Osetrenie automatickej konverzie
    if(t1 == 'F' && t2 == 'I'){
        printf("INT2FLOATS\n");
    }
    else if(t1 == 'I' && t2 == 'F')
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\n");
    printf("LABEL neq$%d\nEQS\nNOTS\n",labneq);
    labneq++;
}

// Generovanie kodu pre operator '=='
static int labeq = 1;
void genEqual(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nPUSHS GF@$2\nPUSHS GF@$1\nJUMPIFEQ eq$%d GF@$1 nil@nil\nJUMPIFEQ eq$%d GF@$2 nil@nil\n", labeq, labeq);
    //Osetrenie automatickej konverzie
    if(t1 == 'F' && t2 == 'I'){
        printf("INT2FLOATS\n");
    }
    else if(t1 == 'I' && t2 == 'F')
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\n");
    printf("LABEL eq$%d\nEQS\n", labeq);
    labeq++;
}

// Generovanie kodu pre operator '<'
void genLower(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == 'F' && t2 == 'I'){
        printf("INT2FLOATS\n");
    }
    else if(t1 == 'I' && t2 == 'F')
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\n");
    printf("LTS\n");
}

// Generovanie kodu pre operator '>'
void genGreater(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == 'F' && t2 == 'I'){
        printf("INT2FLOATS\n");
    }
    else if(t1 == 'I' && t2 == 'F')
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\n");
    printf("GTS\n");
}

// Generovanie kodu pre operator '<='
void genLowerEqual(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == 'F' && t2 == 'I'){
        printf("INT2FLOATS\n");
    }
    else if(t1 == 'I' && t2 == 'F')
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\n");
    printf("POPS GF@$2\nPOPS GF@$1\nLT GF@$p GF@$1 GF@$2\nEQ GF@$d GF@$1 GF@$2\nOR GF@$p GF@$p GF@$d\nPUSHS GF@$p\n");
}

// Generovanie kodu pre operator '>='
void genGreaterEqual(char t1, char t2){
    //Osetrenie hodnot nil
    printf("POPS GF@$1\nPOPS GF@$2\nJUMPIFEQ exit$8 GF@$1 nil@nil\nJUMPIFEQ exit$8 GF@$2 nil@nil\nPUSHS GF@$2\nPUSHS GF@$1\n");
    //Osetrenie automatickej konverzie
    if(t1 == 'F' && t2 == 'I'){
        printf("INT2FLOATS\n");
    }
    else if(t1 == 'I' && t2 == 'F')
        printf("POPS GF@$1\nINT2FLOATS\nPUSHS GF@$1\n");
    printf("POPS GF@$2\nPOPS GF@$1\nGT GF@$p GF@$1 GF@$2\nEQ GF@$d GF@$1 GF@$2\nOR GF@$p GF@$p GF@$d\nPUSHS GF@$p\n");
}

// Generovanie kodu pre prikaz "return"
void genReturn(){
    printf("POPFRAME\nRETURN\n");
}

// Pomocna funkcia pre generovanie kodu pre automaticke konverzie
void genIntToFloats(){
    printf("INT2FLOATS\n");
}
