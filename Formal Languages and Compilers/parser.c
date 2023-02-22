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

#include "scanner.h"
#include "code.h"
#include "errors.h"
#include "expr.h"

symtable_t st;

// Pomocna funkcia na vytvorenie kopie retazca
char* copyString(char* string)
{   
    char* new;
    int length = strlen(string)+1;
    new = malloc(length * sizeof(char));
    if(new == NULL)
    {
        fprintf(stderr, "Internal error (malloc)\n");
        call_error(INTERNAL_ERROR);
    }
    strcpy(new, string);
    return new;
}

// Globalna premenna na cislovanie blokov pri vnarani
int blk_id = 0;

// Funkcia volana po ukonceni bloku, vola funkciu tabulky symbolov, aby vymazala vsetky premenne s danym cislom bloku
void block_end(int blk){
    stBlockEnd(&st, blk);
}

// Globalna premenna, dopredu precitany token (pre prediktivnu syntakticku analyzu)
Token_t token;
// Pomocne globalne premenne pre generator kodu
int mainLabel = 0;          // Pre generovanie navesti v hlavnom programe
int whileLabel = 0;         // Pre generovanie navesti pre prikaz while
int ifLabel = 0;            // Pre generovanie navesti pre prikaz if
char* glob_ret;             // Na uchovanie typu navratovych hodnot funkcie

/////////// Zasobnik mien
char* glob_param[100];      // Globalny zasobnik pre mena premennych a parametrov 
int gpix = 0;

// Vlozenie parametra do zásobníka
void push_param(char* name){
    glob_param[gpix] = name;
    gpix++;
}

// Vyber parametra zo zásobníka
char* pop_param(){
    gpix--;
    return glob_param[gpix];
}
///////////

// Chybove hlasenie syntaktickej chyby
void syn_error(char* msg){
    fprintf(stderr, "%s\n", msg);
    call_error(SYNTAX_ERROR);
}

// Prototypy
void Block();
void ArgumentList();
void Expression();
void WriteCall();

// Procedura na rozpoznanie pravidla: Prolog -> keywordRequire string
void Prolog(){
    if(token.type == keywordRequire){
        token = getToken();
    }
    else{
        syn_error("Expected 'require'");
    }
    if(token.type == string){
        if(strcmp(token.attribute, "ifj21") != 0){
            fprintf(stderr, "Ocakava sa 'ifj21'\n");
            call_error(OTHER_SEM_ERROR);
        }
        token = getToken();
    }
    else{
        syn_error("Expected 'string'");
    }
}
// Procedura na rozpoznanie pravidla: ArgumentList -> [ Expression { comma Expression } ]
void ArgumentList(){
    if(token.type == string || token.type == integer || token.type == number || token.type == keywordNil || token.type == id){
        Expression();
        while(token.type == comma){
            token = getToken();
            Expression();
        }
    }
}

// Procedura na rozpoznanie pravidla: Type -> keywordInteger | keywordString | keywordNumber
void Type(){
    switch(token.type){
        case keywordInteger:
            typestack_push('I');
            token = getToken();
            break;
        case keywordString:
            typestack_push('S');
            token = getToken();
            break;
        case keywordNumber:
            typestack_push('F');
            token = getToken();
            break;
        default:
            syn_error("Expected type");
            break;
    }
}

// Procedura na rozpoznanie pravidla: TypeList -> [ Type { comma Type } ]
void TypeList(){
    if(token.type == keywordInteger || token.type == keywordString || token.type == keywordNumber){
        Type();
        while(token.type == comma){
            token = getToken();
            Type();
        }
    }
}

// Procedura na rozpoznanie pravidla: ParameterList -> [ id colon Type { comma id colon Type } ]
void ParameterList(){
    char t;
    char* name;
    if(token.type == id){
        name = copyString(token.attribute);
        push_param(name);
        token = getToken();
        if(token.type == colon)
            token = getToken();
        else
            syn_error("Expected ':'");
        Type();
        t = typestack_pop();
        stInsertVar(&st,name, t, blk_id + 1);       // Aby mal rovnake cislo ako lokal. premenne
        typestack_push(t);                          // Aby ostalo v zasobniku pre funkciu
        while(token.type == comma){
            token = getToken();
            if(token.type == id){
                name = copyString(token.attribute);
                push_param(name);
                token = getToken();
            }
            else 
                syn_error("Expected identifier");
            if(token.type == colon)
                token = getToken();
            else
                syn_error("Expected ':'");
            Type();
            t = typestack_pop();
            stInsertVar(&st,name, t, blk_id + 1);   // Aby mal rovnake cislo ako lokal. premenne
            typestack_push(t);                      // Aby ostalo v zasobniku pre funkciu
        }
    }
}

// Procedura na rozpoznanie pravidla: VariableDefinition -> keywordLocal id colon Type [ equals Expression ]
void VariableDefinition(){
    char* name;
    char t;
    char texpr;
    token = getToken();
    if(token.type == id){
        name = copyString(token.attribute);
        token = getToken();
    }
    else 
        syn_error("Expected identifier");
    if(token.type == colon)
        token = getToken();
    else
        syn_error("Expected ':'");
    Type();
    t = typestack_pop();
    if(token.type == equals){
        token = getToken();
        Expression();
        texpr = typestack_pop();
        if(t == 'F' && texpr == 'I'){
            genIntToFloats();
            texpr = 'F';
        }
        else if(texpr != 'N' && t != texpr){
            //chyba 4
            fprintf(stderr, "Type incompatibility of assignment\n");
            call_error(ASSIGNMENT_SEM_ERROR);
        }
    }
    else{
        genNil();
    }
    stInsertVar(&st,name, t, blk_id);
    genVarDef(name, blk_id);
    genVarAssign(name, blk_id);
}

// Procedura na rozpoznanie pravidla: ExpressionList -> Expression { comma Expression }
void ExpressionList(){
    Expression();
    while(token.type == comma){
        token = getToken();
        Expression();
    }
}

// Procedura na rozpoznanie pravidla: ExpressionListOptional ->[ Expression { comma Expression } ] 
void ExpressionListOptional(){
    char t;
    unsigned int i = 0;
    if(token.type == string || token.type == integer || token.type == number || token.type == keywordNil || token.type == id || token.type == lBracket || token.type == hash){
        Expression();
        t = typestack_pop();
        if(t == 'I' && i < strlen(glob_ret) && glob_ret[i] == 'F'){
            genIntToFloats();
            typestack_push('F');
        }
        else
            typestack_push(t);
        while(token.type == comma){
            token = getToken();
            Expression();
            i++;
            t = typestack_pop();
            if(t == 'I' && i < strlen(glob_ret) && glob_ret[i] == 'F'){
                genIntToFloats();
                typestack_push('F');
            }
            else
                typestack_push(t);
        }
    }
}

// Procedura na rozpoznanie pravidla: StatementIf -> keywordIf Expression keywordThen Block keywordElse Block keywordEnd
void StatementIf(){
    int label = ifLabel++;
    token = getToken();
    Expression();
    genIfExpr(label);
    (void)typestack_pop();
    if(token.type == keywordThen)
        token = getToken();
    else 
        syn_error("Expected 'then'");
    Block();
    genIfThenElse(label);
    if(token.type == keywordElse)
        token = getToken();
    else 
        syn_error("Expected 'else'");
    Block();
    if(token.type == keywordEnd)
        token = getToken();
    else
        syn_error("Expected 'end'");
    genIfEnd(label);
}

// Procedura na rozpoznanie pravidla: StatementWhile -> keywordWhile Expression keywordDo Block keywordEnd
void StatementWhile(){
    int label = whileLabel++;
    genWhileStart(label);
    token = getToken();
    Expression();
    (void)typestack_pop();
    genWhileExpr(label);
    if(token.type == keywordDo)
        token = getToken();
    else 
        syn_error("Expected 'do'");
    Block();
    if(token.type == keywordEnd)
        token = getToken();
    else
        syn_error("Expected 'end'");
    genWhileEnd(label);
}

// Procedura na rozpoznanie pravidla: AssignmentOrCall -> WriteCall | id ( lBracket ArgumentList rBracket | { comma id } equals ExpressionList) 
void AssignmentOrCall(){
    char* fnc_arg;
    if(strcmp(token.attribute, "write")==0){
        WriteCall();
        return;
    }
    char* idName = copyString(token.attribute);
    token = getToken();
    if(token.type == lBracket){                 //Volanie funkcie
        token = getToken();
        ArgumentList();
        if(token.type == rBracket)
            token = getToken();
        else
            syn_error("Expected ')'");
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
        // Kontrola parametrov
        bstNode_t* sd = stFind(&st, idName);
        if(sd == NULL){
            //chyba 3
            fprintf(stderr, "Function %s is not defined nor declared\n", idName);
            call_error(PROGRAM_SEM_ERROR);
        }
        else if(sd->kind == kVariable){
            //chyba 3
            fprintf(stderr, "%s is not function\n", idName);
            call_error(PROGRAM_SEM_ERROR);
        }
        else if(sd->defined == 0){
            //chyba 3
            fprintf(stderr, "Function %s is not defined\n", idName);
            call_error(PROGRAM_SEM_ERROR);
        }
        // Netreba kontrolovat kFunction
        if(strcmp(fnc_arg, sd->arg_type) != 0){
            fprintf(stderr, "Function %s has incompatible arguments\n", idName);
            call_error(PARAM_AMOUNT_SEM_ERROR);
        }
        // Uvolnenie navratovych typov
        cnt = typestack_items();
        for(int i = cnt - 1; i >= 0; i--){
            (void)typestack_pop();
        }
        sd->used = 1;
        genFncCall(idName);
        genDiscard(strlen(sd->ret_type));
        free(idName);
    }
    else{
        int idcnt = 1;
        push_param(idName);
        while(token.type == comma){
            token = getToken();
            if(token.type == id){
                idName = copyString(token.attribute);
                push_param(idName);
                idcnt++;
                token = getToken();
            }
            else 
                syn_error("Expected identifier");
        }
        if(token.type == equals)
            token = getToken();
        else
            syn_error("Expected '='");
        ExpressionList();
        int cnt = typestack_items();
        if(cnt != idcnt){
            fprintf(stderr, "Assignment has different number of expressions\n");
            call_error(ASSIGNMENT_SEM_ERROR);
        }
        for(int i = cnt - 1; i >= 0; i--){
            char t = typestack_pop();
            char* name = pop_param();
            bstNode_t* sd = stFind(&st, name);
            if(sd == NULL || sd->kind != kVariable){
                fprintf(stderr, "ID %s is not variable\n", name);
                call_error(PROGRAM_SEM_ERROR);
            }
            if(t == 'I' && sd->type == 'F'){
                genIntToFloats();
                t = 'F';
            }
            if(/*t != 'N' &&*/ sd->type != t){
                fprintf(stderr, "Incompatible expression for id %s\n", name);
                call_error(ASSIGNMENT_SEM_ERROR);
            }
            genVarAssign(name, sd->block_id);
        }

    }
}

// Procedura na rozpoznanie pravidla: StatementReturn -> keywordReturn ExpressionListOptional
void StatementReturn(){
    char* fnc_arg;
    token = getToken();
    ExpressionListOptional();
    int cnt = typestack_items();
    fnc_arg = (char*)malloc(cnt + 1);
    if(fnc_arg == NULL){
        fprintf(stderr, "Internal error (malloc)\n");
        call_error(INTERNAL_ERROR);
        return;
    }
    for(int i = cnt - 1; i >= 0; i--){
        fnc_arg[i] = typestack_pop();
    }
    fnc_arg[cnt] = '\0';
    if(glob_ret != NULL){
        if(strlen(glob_ret) < strlen(fnc_arg)){
            fprintf(stderr, "Too many return values\n");
            call_error(PARAM_AMOUNT_SEM_ERROR);
        }
        else {
            for(unsigned int i = 0; i < strlen(fnc_arg); i++){
                if(fnc_arg[i] != 'N' && glob_ret[i] != fnc_arg[i]){
                    fprintf(stderr, "Incompatible types of parameters\n");
                    call_error(PARAM_AMOUNT_SEM_ERROR);
                }
            }
            if(strlen(glob_ret) > strlen(fnc_arg)){
                // Treba dogenerovat nil
                for(unsigned int i = 0; i < strlen(glob_ret) - strlen(fnc_arg); i++){
                    genNil();
                } 
            }
        }
    }
    genReturn();
}

// Procedura na rozpoznanie pravidla: Statement -> VariableDefinition | StatementIf | StatementWhile | AssignmentOrCall | StatementReturn
void Statement(){
    switch(token.type){
        case keywordLocal:
            VariableDefinition();
            break;
        case keywordIf:
            StatementIf();
            break;
        case keywordWhile:
            StatementWhile();
            break;
        case id:
            AssignmentOrCall();
            break;
        case keywordReturn:
            StatementReturn();
            break;
        default:
            syn_error("Expected statement");
            break;
    }
}

// Procedura na rozpoznanie pravidla: Block -> { Statement }
void Block(){
    int blk = ++blk_id;
    while(token.type == keywordLocal || token.type == keywordIf || token.type == keywordWhile || token.type == id || token.type == keywordReturn){
        Statement();
    }
    block_end(blk);
}

// Procedura na rozpoznanie pravidla: FunctionDeclaration -> keywordGlobal id colon keywordFunction lBracket TypeList rBracket colon TypeList 
void FunctionDeclaration(){
    char* fnc_arg;
    char* fnc_ret;
    char* name;
    token = getToken();
    if(token.type == id){
        name = copyString(token.attribute);
        token = getToken();
    }
    else
        syn_error("Expecter identifier");
    if(token.type == colon)
        token = getToken();
    else
        syn_error("Expected ':'");
    if(token.type == keywordFunction)
        token = getToken();
    else
        syn_error("Expected 'function'");
    if(token.type == lBracket)
        token = getToken();
    else
        syn_error("Expected '('");
    TypeList();
    // Spracovanie typov argumentov
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
    if(token.type == rBracket)
        token = getToken();
    else
        syn_error("Expected ')'");
    if(token.type == colon){
        token = getToken();
        TypeList();
    }
    cnt = typestack_items();
    fnc_ret = (char*)malloc(cnt + 1);
    if(fnc_ret == NULL){
        fprintf(stderr, "Internal error (malloc)\n");
        call_error(INTERNAL_ERROR);
    }
    for(int i = cnt - 1; i >= 0; i--){
        fnc_ret[i] = typestack_pop();
    }
    fnc_ret[cnt] = '\0';
    stInsertFunc(&st, name, 0, 0, fnc_arg, fnc_ret, 0, 0);
}

// Procedura na rozpoznanie pravidla: FunctionDefinition -> keywordFunction id lBracket ParameterList rBracket [ colon TypeList ] Block keywordEnd
void FunctionDefinition(){
    int lab = mainLabel++;
    char* name;
    char* pname;
    char* fnc_arg;
    char* fnc_ret;
    token = getToken();
    if(token.type == id){
        name = copyString(token.attribute);
        genFncStart(token.attribute, lab);
        token = getToken();
    }
    else
        syn_error("Expected identifier");
    if(token.type == lBracket)
        token = getToken();
    else
        syn_error("Expected '('");
    ParameterList();
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
    for(int i = cnt - 1; i >= 0; i--){
        pname = pop_param();
        genParamAsign(pname);
    }
    if(token.type == rBracket)
        token = getToken();
    else
        syn_error("Expected ')'");
    if(token.type == colon){
        token = getToken();
        TypeList();
    }
    cnt = typestack_items();
    fnc_ret = (char*)malloc(cnt + 1);
    if(fnc_ret == NULL){
        fprintf(stderr, "Internal error (malloc)\n");
        call_error(INTERNAL_ERROR);
    }
    for(int i = cnt - 1; i >= 0; i--){
        fnc_ret[i] = typestack_pop();
    }
    fnc_ret[cnt] = '\0';
    stInsertFunc(&st, name, 0, 1, fnc_arg, fnc_ret, 0, 0);
    glob_ret = copyString(fnc_ret);
    Block();
    if(token.type == keywordEnd)
        token = getToken();
    else
        syn_error("Expected 'end'");
    genFncEnd(name, lab, cnt);
    blk_id = 0;
}

// Procedura na rozpoznanie pravidla: WriteCall -> id lBracket Expression { comma Expression } rBracket  //id = "write"
void WriteCall(){
    token = getToken();
    if(token.type == lBracket)
        token = getToken();
    else
        syn_error("Expected '('");
    Expression();
    genCallWrite();
    (void) typestack_pop();
    while(token.type == comma){
        token = getToken();
        Expression();
        genCallWrite();
        (void) typestack_pop();
    }
    if(token.type == rBracket)
        token = getToken();
    else
        syn_error("Expected ')'");
}

// Procedura na rozpoznanie pravidla: FunctionCall -> WriteCall | id lBracket ArgumentList rBracket
void FunctionCall(){
    char* fnc_arg;
    if(strcmp(token.attribute, "write")==0){
        WriteCall();
        return;
    }
    char* fncName = copyString(token.attribute);
    token = getToken();
    if(token.type == lBracket)
        token = getToken();
    else
        syn_error("Expected '('");
    ArgumentList();
    int cnt = typestack_items();
    fnc_arg = (char*)malloc(cnt + 1);
    if(fnc_arg == NULL){
        fprintf(stderr, "Internal error\n");
        call_error(INTERNAL_ERROR);
    }
    for(int i = cnt - 1; i >= 0; i--){
        fnc_arg[i] = typestack_pop();
    }
    fnc_arg[cnt] = '\0';
    if(token.type == rBracket)
        token = getToken();
    else
        syn_error("Expected ')'");
    // Kontrola parametrov
    bstNode_t* sd = stFind(&st, fncName);
    if(sd == NULL){
        //chyba 3
        fprintf(stderr, "Function %s is not defined nor declared\n", fncName);
        call_error(PROGRAM_SEM_ERROR);
    }
    else if(sd->kind == kVariable){
        //chyba 3
        fprintf(stderr, "%s is not function\n", fncName);
        call_error(PROGRAM_SEM_ERROR);
    }
    else if(sd->defined == 0){
        //chyba 3
        fprintf(stderr, "Function %s is not defined\n", fncName);
        call_error(PROGRAM_SEM_ERROR);
    }
    // Netreba kontrolovat kFunction
    if(strcmp(fnc_arg, sd->arg_type) != 0){
        fprintf(stderr, "Function %s has incompatible arguments\n", fncName);
        call_error(PARAM_AMOUNT_SEM_ERROR);
    }
    // Uvolnenie navratovych typov
    cnt = typestack_items();
    for(int i = cnt - 1; i >= 0; i--){
        (void)typestack_pop();
    }
    sd->used = 1;
    genFncCall(fncName);
    genDiscard(strlen(sd->ret_type));
    free(fncName);
}

// Procedura na rozpoznanie pravidla: Program -> Prolog { ( FunctionDeclaration | FunctionDefinition | FunctionCall ) }
void Program(){
    Prolog();
    genMain_start();
    while(token.type == keywordGlobal || token.type == keywordFunction || token.type == id){
        if(token.type == keywordGlobal){
            FunctionDeclaration();
        }
        else if(token.type == keywordFunction){
            FunctionDefinition();
        }
        else if(token.type == id){
            FunctionCall();
        }
    }
}

int main(){
    typestack_init(20);
    stInit(&st);
    stInsertFunc(&st, "reads", 0, 1, "", "S", 1, 0);
    stInsertFunc(&st, "readi", 0, 1, "", "I", 1, 0);
    stInsertFunc(&st, "readn", 0, 1, "", "F", 1, 0);
    stInsertFunc(&st, "write", 0, 1, "*", "", 1, 0);
    stInsertFunc(&st, "tointeger", 0, 1, "F", "I", 1, 0);
    stInsertFunc(&st, "substr", 0, 1, "SII", "S", 1, 0);
    stInsertFunc(&st, "ord", 0, 1, "SI", "I", 1, 0);
    stInsertFunc(&st, "chr", 0, 1, "I", "S", 1, 0);
    token = getToken();
    Program();
    if(token.type != start){
        syn_error("Expected EOF");
    }
    genExit();
    bstNode_t* node;
    // Zistit, ktore vstavane funkcie boli pouzite a tie dat na vystup
    genWrite();
    if((node = stFind(&st, "readi")) != NULL && node->used == 1)
        genReadi();
    if((node = stFind(&st, "reads")) != NULL && node->used == 1)
        genReads();
    if((node = stFind(&st, "readn")) != NULL && node->used == 1)
        genReadn();
    if((node = stFind(&st, "tointeger")) != NULL && node->used == 1)
        genTointeger();
    if((node = stFind(&st, "substr")) != NULL && node->used == 1)
        genSubstr();
    if((node = stFind(&st, "ord")) != NULL && node->used == 1)
        genOrd();
    if((node = stFind(&st, "chr")) != NULL && node->used == 1)
        genChr();
}
