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
#include "symtable.h"
#include "errors.h"

/* FUNKCE PRO BST */
void bstInit(bstNode_t **tree) {
  *tree = NULL;
}

bstNode_t* bstSearch(bstNode_t *tree, char* name) {
  
  //pokud je strom prázdný
  if(tree == NULL)
  {
    return NULL;
  }

  if(strcmp(tree->name,name) == 0)
  {
    return tree;
  }
  
  if (strcmp(name,tree->name) < 0) //levá větev
  {
    return bstSearch(tree->left, name);
  }
  else //pravá větev
  {
    return bstSearch(tree->right, name);
  }
  return NULL;
}


void bstInsertVar(bstNode_t **tree, char* name, char type, int block_id) {
  //vložení listu
  if((*tree) == NULL)
  {
    (*tree) = (bstNode_t*) malloc(sizeof(bstNode_t));
    if((*tree) == NULL)
    {
      call_error(INTERNAL_ERROR);
    }
    
    (*tree)->name = name;
    (*tree)->kind = kVariable;
    (*tree)->type = type;
    (*tree)->block_id = block_id;  

    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->hidden = NULL;
    return;
  }
 
  if((strcmp(name,(*tree)->name) == 0) && (*tree)->kind == kFunction){
    fprintf(stderr, "Funkcia %s je uz deklarovana\n", name);
    call_error(PROGRAM_SEM_ERROR);
  }

  //nahrazení uzlu se stejným klíčem a stejným id
  if((strcmp(name,(*tree)->name) == 0) && ((*tree)->block_id == block_id))
  {
    //fprintf(stderr, "Vkladam %s\n", name);
    call_error(PROGRAM_SEM_ERROR);
  }
  
  if((strcmp(name,(*tree)->name) == 0) && ((*tree)->block_id != block_id))
  {
    bstNode_t* tmp = malloc(sizeof(bstNode_t));
    if(tmp == NULL)
    {
      call_error(INTERNAL_ERROR);
    }

    tmp->name = (*tree)->name;
    tmp->kind = (*tree)->kind;
    tmp->type = (*tree)->type;
    tmp->block_id = (*tree)->block_id;
    tmp->hidden = (*tree)->hidden;

    (*tree)->name = name;
    (*tree)->kind = kVariable;
    (*tree)->type = type;
    (*tree)->block_id = block_id;   

    (*tree)->hidden = tmp;
    return; 
  }

  if(strcmp(name,(*tree)->name) < 0)//levá větev
  {
    bstInsertVar(&((*tree)->left), name, type, block_id); 
  }
  else //pravá větev
  {
    bstInsertVar(&((*tree)->right), name, type, block_id); 
  }
}



void bstInsertFunc(bstNode_t **tree, char* name, int block_id, int defined, char* arg_type, char* ret_type, int built_in, int used) {

  //vložení listu
  if((*tree) == NULL)
  {
    (*tree) = (bstNode_t*) malloc(sizeof(bstNode_t));
    if((*tree) == NULL)
    {
      call_error(INTERNAL_ERROR);
    }
    
    (*tree)->name = name;
    (*tree)->kind = kFunction;
    (*tree)->block_id = block_id;
    (*tree)->defined = defined;
    (*tree)->arg_type = arg_type;
    (*tree)->ret_type = ret_type;  
    (*tree)->built_in = built_in;
    (*tree)->used = used;    

    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->hidden = NULL;
    return;
  }
 

  //funkce již existuje
  if((strcmp(name,(*tree)->name) == 0))
  {
    if((*tree)->defined == 1){
      fprintf(stderr, "Function %s is already defined\n", name);
      call_error(PROGRAM_SEM_ERROR);
    }
    else if((*tree)->defined == 0 && defined == 0){
      fprintf(stderr, "Function %s is already declared\n", name);
      call_error(PROGRAM_SEM_ERROR);
    }
    else{
      if(strcmp((*tree)->arg_type, arg_type) != 0 || strcmp((*tree)->ret_type, ret_type) != 0){
        fprintf(stderr, "Function %s has different header than in declaration\n", name);
        call_error(PROGRAM_SEM_ERROR);
      }
      (*tree)->defined = 1;
      return;
    }
  }
  if(strcmp(name,(*tree)->name) < 0)//levá větev
  {
    bstInsertFunc(&((*tree)->left), name, block_id, defined, arg_type, ret_type, built_in, used); 
  }
  else //pravá větev
  {
    bstInsertFunc(&((*tree)->right), name, block_id, defined, arg_type, ret_type, built_in, used); 
  }
}


void bst_replace_by_rightmost(bstNode_t *target, bstNode_t **tree) {

  //pokud stále existuje nějaký pravý list, zavolám funkci znovu pro pravou větev uzlu
  if((*tree)->right != NULL)
  {
    bst_replace_by_rightmost(target, &(*tree)->right);
  }
  else
  {
    //nahrazení hodnot  
    target->name = (*tree)->name;
    target->kind = (*tree)->kind;
    target->block_id = (*tree)->block_id;
    target->hidden = (*tree)->hidden;

    if((*tree)->kind == kVariable)
    {
      target->type = (*tree)->type;
    }
    else if((*tree)->kind == kFunction)
    {
      target->defined = (*tree)->defined;
      target->arg_type = (*tree)->arg_type;
      target->ret_type = (*tree)->ret_type;  
      target->built_in = (*tree)->built_in;
      target->used = (*tree)->used; 
    }
    else
    {
      call_error(PROGRAM_SEM_ERROR);
    }

    //nahrání levého listu. Pravý nemá smysl, protože v téhle fázi jsem v nejpravějším listu
    (*tree) = (*tree)->left;   
  }  
}

void bstDelete(bstNode_t **tree, char* name) {
  bstNode_t *tmp;
  //pokud je strom prázdný, nestane se nic
  if((*tree) == NULL)
  {
    return;
  } 

  //hledání uzlu
  if(strcmp(name,(*tree)->name) < 0)//levá větev
  {
		bstDelete(&((*tree)->left), name);
  }

  if(strcmp(name,(*tree)->name) > 0)//pravá větev
  {
		bstDelete(&((*tree)->right), name);
  }

  if(strcmp(name, (*tree)->name) != 0)
    return;
  
  //uzel, ktery ma prekryte 
  if((*tree)->hidden != NULL)
  {
    tmp = (*tree);
    (*tree) = (*tree)->hidden;
    free(tmp);
    tmp = NULL;
    return;
  }
  //uzel, který nemá potomky
  else if((*tree)->left == NULL && (*tree)->right == NULL)
  {
    free((*tree));
    (*tree) = NULL;
  }
  //uzel, který má pouze levého potomka
  else if((*tree)->right == NULL)
    {
      tmp = (*tree)->left;
      free((*tree));
      (*tree) = tmp;
    }
  //uzel, který má pouze pravého potomka
  else if((*tree)->left == NULL)
  {
    tmp = (*tree)->right;
    free((*tree));
    (*tree) = tmp;
  }
  //uzel, který má oba potomky
  else
  {  
    tmp = (*tree);
    bst_replace_by_rightmost(tmp, &(tmp)->left);    
    (*tree) = tmp; 
  }
}


void bstDispose(bstNode_t **tree) {
  
  if((*tree) != NULL)
  {
    bstDispose(&(*tree)->left);
    bstDispose(&(*tree)->right);
    bstDispose(&(*tree)->hidden);

    free((*tree));
    (*tree) = NULL;
  }
  
}

void bstEndBlockDelete(bstNode_t **tree, int blk_id){
  if((*tree) != NULL){
    bstEndBlockDelete(&(*tree)->left, blk_id);
    bstEndBlockDelete(&(*tree)->right, blk_id);
    if(blk_id == (*tree)->block_id){
      //printf("mazem blok %d, id = %s, blk = %d\n",blk_id, (*tree)->name, (*tree)->block_id);
      bstDelete(tree, (*tree)->name);
    }
  }
}

void bstPrint(bstNode_t *tree){ //ladiaca funkcia
  
  if(tree != NULL)
  {
    bstPrint(tree->left);
    printf("id = %s, blk = %d\n", tree->name, tree->block_id);
    bstPrint(tree->right);
  }
  
}
//--------------------------------------------------
/* FUNKCE PRO SYMTABLE */

void stInit(symtable_t* table)
{
  bstInit(&(table)->tree);
}

void stPrint(symtable_t* table){ //ladiaca funkcia
  bstPrint(table->tree);
}

bstNode_t* stFind(symtable_t* table, char* name)
{
  bstNode_t* found = bstSearch(table->tree, name);
  if(found != NULL)
  {
    return found;
  }
  return NULL;
}



void stInsertVar(symtable_t* table, char* name, char type, int blk_id)
{
  bstInsertVar(&(table)->tree, name, type, blk_id); 
}

void stInsertFunc(symtable_t* table, char* name, int blk_id, int defined, char* arg_type, char* ret_type, int built_in, int used)
{
  bstInsertFunc(&(table)->tree, name, blk_id, defined, arg_type, ret_type, built_in, used); 
}

void stDelete(symtable_t* table, char* name)
{
  bstDelete(&(table)->tree, name);
}

void stDispose(symtable_t* table)
{
  bstDispose(&(table)->tree);
}



void stBlockEnd(symtable_t* table, int blk_id){
  bstEndBlockDelete(&(table)->tree, blk_id);
}