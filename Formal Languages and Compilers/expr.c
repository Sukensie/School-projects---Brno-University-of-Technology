/*
Projekt do předmětů IFJ a IAL (ZS 2021)
Tým 031, varianta I

Autoři:
xploci01: Peter Pločica
xsouce15: Tomáš Souček
xcolog00: Adam Cologna
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "scanner.h"
#include "errors.h"
#include "expr.h"
#include "code.h"

//////////////////////////////// Typova kontrola 
struct{         //typovy zasobnik
    char* data;
    int size;
    int ptr;    //index vrcholu
}typestack;

/*
    Inicializacia typ. zasobnika
    @param size: pociatocna velkost
*/
void typestack_init(int size){ 
    typestack.data = (char*)malloc(size);
    if(typestack.data == NULL){
        fprintf(stderr, "Internal error (malloc)\n");
        call_error(INTERNAL_ERROR);
    }
    typestack.size = size;
    typestack.ptr = 0;
}

/*
    Vlozenie typu do zasobnika
    @param type: pridavany typ
*/
void typestack_push(char type){
    if(typestack.size == typestack.ptr + 1){    //realokacia na v
        typestack.data = (char*)realloc(typestack.data, 2*typestack.size);
        if(typestack.data == NULL){
            fprintf(stderr, "Internal error (realloc)\n");
            call_error(INTERNAL_ERROR);
        }
        typestack.size = 2*typestack.size;
    }
    typestack.data[typestack.ptr] = type;
    typestack.ptr++;
}

/*
    Vyber hodnoty zo zasobnika
    @return: typ
*/
char typestack_pop(){
    if(typestack.ptr == 0){
        fprintf(stderr, "Internal Error\n");
        call_error(INTERNAL_ERROR);
    }
    typestack.ptr--;
    return typestack.data[typestack.ptr];
}

/*
    Vrati pocet poloziek v zasobniku
    @return: pocet
*/
int typestack_items(){
    return typestack.ptr;
}

//////////////////////////////// Precedencna syntakticka analyza
//konstanty
#define EXPR 99
#define LTSYM 77 

/*
    Prevod kodu tokenu do kodu PSA tabulky
    @param type: kod tokenu
    @return: kod PSA tabulky
*/
int tokencode(int type){
    switch(type){
        case hash:
            return 0;
        case multiply:
        case divide:
        case intDivide:
            return 1;
        case plus:
        case minus:
            return 2;
        case concatenate:
            return 3;
        case greater:
        case lower:
        case greaterEqual:
        case lowerEqual:
        case notEqual:
        case doesEqual:
            return 4;
        case lBracket:
            return 5;
        case rBracket:
            return 6;
        case id:
            return 7;
        case integer:
        case string:
        case number:
        case keywordNil:
            return 8;
        case comma:
            return 9;
        default:
            return 10;
    }
}

//Zasobnik PSA
typedef struct {
    int array[50];
    int tokenarray[50];
    char* atrarray[50];
    int topIndex;
} Stack_t;

Stack_t psa_stack;

/*
    Inicializacia zasobnika PSA
*/
void psaInit(){
    psa_stack.topIndex = 0;
}

/*
    Vlozenie tokenu do zasobnika PSA
    @param sym: kod PSA tabulky
    @param type: kod tokenu
    @param atr: atribut tokenu
*/
void psaPush(int sym, int type, char* atr){
    psa_stack.array[psa_stack.topIndex] = sym;
    psa_stack.tokenarray[psa_stack.topIndex] = type;
    psa_stack.atrarray[psa_stack.topIndex] = atr;
    psa_stack.topIndex++;
}

/*
    Vrati vrchny terminal zo zasobnika PSA
    @return: kod PSA tabulky pre terminal
*/
int psaTopTerm(){
    for(int i = psa_stack.topIndex -1; i >= 0; i--){
        if(psa_stack.array[i] < 11)
            return psa_stack.array[i];
    }
    return 10;
}

/*
    Doplnenie '<' za vrchny terminal zasobnika
*/
void psaAlternTopTerm(){
    for(int i = psa_stack.topIndex; i >= 0; i--){
        psa_stack.array[i] = psa_stack.array[i-1];
        psa_stack.tokenarray[i] = psa_stack.tokenarray[i-1];
        psa_stack.atrarray[i] = psa_stack.atrarray[i-1];
        if(psa_stack.array[i] < 11){
            psa_stack.array[i] = LTSYM;
            psa_stack.tokenarray[i] = LTSYM;
            psa_stack.topIndex++;
            return;
        }
    }
}

/*
    Redukcia pravidla v zasobniku PSA
    Zaroven sa vola typova kontrola aj generovanie kodu
*/
void psaFindRight(){
    int i;
    char t1, t2;
    bstNode_t* sd;
    for(i = psa_stack.topIndex -1; i >= 0; i--){
        if(psa_stack.array[i] == LTSYM)
            break;
    }
    int len = psa_stack.topIndex - i - 1;
    psa_stack.array[i] = EXPR;
    psa_stack.topIndex = i + 1;
    //elementarne vyrazy
    if(len == 1){
        switch(psa_stack.tokenarray[i+1]){
            case string:
            typestack_push('S');
            genString(psa_stack.atrarray[i+1]);
            break;
        case integer:
            typestack_push('I');
            genNumberInt(psa_stack.atrarray[i+1]);
            break;
        case number:
            typestack_push('F');
            genNumberFloat(psa_stack.atrarray[i+1]);
            break;
        case keywordNil:
            typestack_push('N');
            genNil();
            break;
        case id:
            sd = stFind(&st, psa_stack.atrarray[i+1]);
            if(sd == NULL || sd->kind != kVariable){
                fprintf(stderr, "ID %s is not variable\n", psa_stack.atrarray[i+1]);
                call_error(PROGRAM_SEM_ERROR);
            }
            else{
                typestack_push(sd->type);
                genVarUse(sd->name, sd->block_id);
            }
            break;
        default:
            fprintf(stderr, "Expected operand\n");
            call_error(SYNTAX_ERROR);
            break;
        }

    }
    //vyraz # EXPR
    else if(len == 2 && psa_stack.tokenarray[i+1] == hash && psa_stack.array[i+2] == EXPR){
        t1 = typestack_pop();
        if(t1 != 'S'){
            fprintf(stderr, "Type incompatibility of operand\n");
            call_error(SEM_COMPATIBILITY_ERROR);
        }
        typestack_push('I');
        genLength();
    }
    // ( EXPR )
    else if(len == 3 && psa_stack.array[i+2] == EXPR && psa_stack.tokenarray[i+1] == lBracket && psa_stack.tokenarray[i+3] == rBracket){
        //nic netreba
    }
    // volanie funkcie f ( EXPR, ... )
    else if(len >= 3 && psa_stack.tokenarray[i+1] == id && psa_stack.tokenarray[i+2] == lBracket && psa_stack.tokenarray[i+len] == rBracket){
        char* name = psa_stack.atrarray[i+1];
        char* fnc_arg;
        sd = stFind(&st, psa_stack.atrarray[i+1]);
        if(sd == NULL){
            fprintf(stderr, "ID %s not found\n", name);
            call_error(PROGRAM_SEM_ERROR);
        }
        int cnt = typestack_items();
        fnc_arg = (char*)malloc(cnt + 1);
        if(fnc_arg == NULL){
            fprintf(stderr, "Internal error (malloc)\n");
            call_error(INTERNAL_ERROR);
        }
        for(int i = cnt - 1; i >= 0; i--){
            fnc_arg[i] = typestack_pop();
        }
        fnc_arg[cnt] = '\0';
        if(sd->kind != kFunction){
            fprintf(stderr, "Variable %s used as function\n", name);
            call_error(PROGRAM_SEM_ERROR);
        }
        if(strlen(sd->ret_type) == 0){
            fprintf(stderr, "Void function %s used in expression\n", name);
            call_error(OTHER_SEM_ERROR);
            }
        for(unsigned int i = 0; i < strlen(sd->ret_type); i++){
            typestack_push(sd->ret_type[i]);
        }
        if(strlen(sd->arg_type) != (unsigned int)cnt){
            fprintf(stderr, "Function %s called with wrong number of arguments\n", name);
            call_error(PARAM_AMOUNT_SEM_ERROR);
        }
        for(unsigned int i = 0; i < strlen(sd->arg_type); i++){
            if(/*fnc_arg[i] != 'N' &&*/ fnc_arg[i] != sd->arg_type[i]){
                //chyba 5
                fprintf(stderr, "Function %s has incompatible arguments\n", name);
                call_error(PARAM_AMOUNT_SEM_ERROR);
            }
        }
        sd->used = 1;
        genFncCall(name);
    }
    //vyraz s binarnym operatorom: EXPR operator EXPR 
    else if(len == 3 && psa_stack.array[i+1] == EXPR && psa_stack.array[i+3] == EXPR){
        int operator = psa_stack.tokenarray[i+2];
        switch(operator){
            case plus:
            case minus:
                t2 = typestack_pop();
                t1 =  typestack_pop();
                if((t1 == 'I' || t1 == 'F') && (t2 == 'I' || t2 == 'F')){
                    if(t1 == 'I' && t2 == 'I')
                        typestack_push('I');
                    else{
                        typestack_push('F');
                    }
                }
                else{
                    fprintf(stderr, "Incompatible subexpressions\n");
                    call_error(SEM_COMPATIBILITY_ERROR);
                }
                if(operator == plus)
                    genPlus(t1, t2);
                else
                    genMinus(t1, t2);
                break;
            case multiply:
            case divide:
            case intDivide:
                t2 = typestack_pop();
                t1 =  typestack_pop();
                if(operator == intDivide){
                    if(t1 != 'I' || t2 != 'I'){
                        fprintf(stderr, "Incompatible subexpressions\n");
                        call_error(SEM_COMPATIBILITY_ERROR);
                    }
                    else{
                        genIntDivide();
                        typestack_push('I');
                    }
                }
                else if((t1 == 'I' || t1 == 'F') && (t2 == 'I' || t2 == 'F')){
                    if(operator == divide)
                        typestack_push('F');
                    else if(t1 == 'I' && t2 == 'I')
                        typestack_push('I');
                    else{
                        typestack_push('F');
                    }
                }
                else{
                    fprintf(stderr, "Incompatible subexpressions\n");
                    call_error(SEM_COMPATIBILITY_ERROR);
                }
                if(operator == multiply)
                    genMultiply(t1, t2);
                else if(operator == divide)
                    genDivide(t1, t2);
                break;
            case concatenate:
                t2 = typestack_pop();
                t1 = typestack_pop();
                if(t1 != 'S' || t2 != 'S'){
                    fprintf(stderr, "Incompatible subexpressions\n");
                    call_error(SEM_COMPATIBILITY_ERROR);
                }
                typestack_push('S');
                genConcatenate();
                break;
            case greaterEqual:
            case lowerEqual:
            case greater:
            case lower:
            case notEqual:
            case doesEqual:
                t2 =  typestack_pop();
                t1 =  typestack_pop();
                if(t1 == t2 || ((t1 == 'I' || t1 == 'F') && (t2 == 'I' || t2 == 'F')) || ((operator == notEqual || operator == doesEqual) && (t1 == 'N' || t2 == 'N'))){
                    typestack_push('B');
                }
                else{
                    fprintf(stderr, "Incompatible subexpressions\n");
                    call_error(SEM_COMPATIBILITY_ERROR);
                }
                if(operator == notEqual)
                    genNotEqual(t1, t2);
                else if(operator == doesEqual)
                    genEqual(t1, t2);
                else if(operator == lower)
                    genLower(t1, t2);
                else if(operator == greater)
                    genGreater(t1, t2);
                else if(operator == lowerEqual)
                    genLowerEqual(t1, t2);
                else
                    genGreaterEqual(t1, t2);
                break;
        }
    }
}
    
 

extern Token_t token; //spolocny token s parserom

/*
    Precedencna analyza pre vyraz
*/
void Expression(){
    // Precedencna tabulka
    char table[11][11] = { //radky jsou vrchol zasobniku, sloupce jsou input
            //0    1    2    3    4    5    6    7    8    9    10
    /*0*/   {'<', '>', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    /*1*/   {'<', '>', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    /*2*/   {'<', '<', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    /*3*/   {'<', '<', '<', '<', '>', '<', '>', '<', '<', '>', '>'},
    /*4*/   {'<', '<', '<', '<', '<', '<', '>', '<', '<', '>', '>'},
    /*5*/   {'<', '<', '<', '<', '<', '<', '=', '<', '<', '=', 'e'},
    /*6*/   {'>', '>', '>', '>', '>', 'e', '>', '$', 'e', '>', '>'},
    /*7*/   {'>', '>', '>', '>', '>', '=', '>', '$', 'e', '>', '>'},
    /*8*/   {'>', '>', '>', '>', '>', 'e', '>', '$', 'e', '>', '>'},
    /*9*/   {'<', '<', '<', '<', '<', '<', '=', '<', '<', '=', 'e'},
    /*10*/  {'<', '<', '<', '<', '<', '<', '$', '<', '<', '$', 'd'}
        };
    int id_input;
    int id_topstack;
    char* atr;
    psaInit();
    psaPush(10, 0, NULL);
    id_input = 0;
    bool done = false;
    while(!done){
        id_topstack = psaTopTerm();
        ////// Specialne osetrenie konca vyrazu
        if(id_input != 10)
            id_input = tokencode(token.type);
        if(table[id_topstack][id_input] == '$'){
            id_input = 10;
        }
        //////
        if(id_input == 7 || id_input == 8){
            atr = copyString(token.attribute);
        }
        else{
            atr = NULL;
        }
        char output = table[id_topstack][id_input];
        if(output == 'd'){
            break;
        }
        else if(output == '='){
            psaPush(id_input, token.type, atr);
            token = getToken();
        }
        else if(output == '<'){
            psaAlternTopTerm();
            psaPush(id_input, token.type, atr);
            token = getToken();
        } 
        else if(output == '>'){
            psaFindRight();
        }
        else{
            fprintf(stderr, "SYNAX ERROR\n");
            call_error(SYNTAX_ERROR);
        }        
    }
    return;
}

/*  Pomocne main pre testovanie
int main(){
    token = getToken();
    Expressionpsa();
}
*/