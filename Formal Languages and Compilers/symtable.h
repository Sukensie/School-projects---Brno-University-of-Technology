/*
Projekt do předmětů IFJ a IAL (ZS 2021)
Tým 031, varianta I

Autoři:
xploci01: Peter Pločica
xsouce15: Tomáš Souček
xcolog00: Adam Cologna
*/


#define kVariable 1
#define kFunction 2

typedef struct bstNode {
  char* name;              
  int kind;    //var ||function
  char type;   //'I' = int, 'S' = string, 'F' = float, 'N' = nil
  int block_id;
  int defined;
  char* arg_type;
  char* ret_type;
  int built_in;
  int used;

  struct bstNode *left;  
  struct bstNode *right;
  struct bstNode *hidden;
} bstNode_t;

typedef struct symtable {
    bstNode_t* tree;
} symtable_t;


void stInit(symtable_t* table);
void stDispose(symtable_t* table);
bstNode_t* stFind(symtable_t* table, char* name);
void stInsertVar(symtable_t* table, char* name, char type, int blk_id);
void stInsertFunc(symtable_t* table, char* name, int blk_id, int defined, char* arg_type, char* ret_type, int built_in, int used);
void stDelete(symtable_t* table, char* name);
void stPrint(symtable_t* table);  //ladiaca funkcia
void stBlockEnd(symtable_t* table, int blk_id);
