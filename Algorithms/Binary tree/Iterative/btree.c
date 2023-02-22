/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  
  //pokud je strom prázdný
  if(tree == NULL)
  {
    return false;
  }

  //cyklus na projítí celého stromu
  while(tree != NULL)
  {
    if(tree->key == key)
    {
      *value = tree->value;
      return true;
    }
    if(key < tree->key)
    {
      tree = tree->left;//levá větev
    }
    else
    {
      tree = tree->right;//pravá větev
    }
  }

  //pokud došel na konec stromu a nenašel hledaný prvek, vrátí false
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  //pokud je strom prázdný, alokuji místo pro uzel a rovnou jej vložím
  if((*tree) == NULL)
  {  
    (*tree) = malloc(sizeof(bst_node_t));
    if((*tree) == NULL)
    {
      return; //nepovedená alokace
    }
    
    //vložení hodnot
    (*tree)->value = value;
    (*tree)->key = key;
    (*tree)->left = NULL;
    (*tree)->right = NULL;

    return;
  }
 

  bst_node_t * tmp = (*tree);
  while(tmp != NULL)
  {
    //nahrazení uzlu se stejným klíčem
    if(key == tmp->key)
    {
      tmp->value = value;
      return;
    }

    if(key < tmp->key)//levá větev
    {
      //pokud už došel k nejhlubšímu uzlu, vloží prvek
      if(tmp->left == NULL)
      {
        tmp->left =  malloc(sizeof(bst_node_t));
        tmp->left->value = value;
        tmp->left->key = key;
        tmp->left->left = NULL;
        tmp->left->right = NULL;
        return; 
      }
      else
      {
        tmp = tmp->left;
      }
    }
    else //pravá větev
    {
      //pokud už došel k nejhlubšímu uzlu, vloží prvek
      if(tmp->right == NULL)
      {
        tmp->right =  malloc(sizeof(bst_node_t));
        tmp->right->value = value;
        tmp->right->key = key;
        tmp->right->left = NULL;
        tmp->right->right = NULL;
        return;
      }
      else
      {
        tmp = tmp->right;
      }
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  bst_node_t* prev = NULL; //slouží k propojení potomka mazaného uzlu

  //pokud stále existuje nějaký pravý list, zavolám funkci znovu pro pravou větev uzlu
  while((*tree)->right != NULL)
  {
    prev = (*tree);
    (*tree) = (*tree)->right;
  }
  
  //nahrazení hodnot  
  target->key = (*tree)->key;
  target->value = (*tree)->value;

  //propojení potomka mazaného uzlu. Pravý nemá smysl, protože v téhle fázi jsem v nejpravějším listu  
  if((*tree) == prev->right)
  {
    prev->right = (*tree)->left;
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

  //pokud je strom prázdný, nestane se nic
  if((*tree) == NULL)
  {
    return;
  } 

  bst_node_t* curr = (*tree);
  bst_node_t* prev = NULL;

  
  //fáze hledání prvku
  while (curr != NULL && curr->key != key) 
  {
    prev = curr;
    if (key < curr->key)
    {
      curr = curr->left;
    }
    else
    {
      curr = curr->right;
    }  
  }

  //ukončí provádění funkce, pokud je strom prázdný, nebo nenešel položku, kterou chci mazat
  if (curr == NULL) 
  {
    return;
  }

  //uzel, který nemá potomky
  if(curr->left == NULL && curr->right == NULL)
  { 
    //pokud je mazaný prvek root
    if (prev == NULL)
    {
      free((*tree));
      (*tree) = NULL;
      return;
    }

    //smazání ukazatelů na mazaný uzel
    if(curr == prev->left)
    {
      prev->left = NULL;
    }
    else
    {
      prev->right = NULL;
    }
    free(curr);
    return;
  }

  //uzel, který má pouze levého potomka
  else if(curr->right == NULL)
  {
    bst_node_t * tmp = curr->left;

    //pokud jde o root
    if(prev == NULL)
    {
      (*tree)= tmp;
      free(curr);
      return;
    }
    
    //aktualizace ukazatelů na potomka mazaného uzlu
    if(curr == prev->left)
    {
      prev->left = tmp;
    }
    else
    {
      prev->right = tmp;
    }
    free(curr);
    return;
  }

  //uzel, který má pouze pravého potomka
  else if(curr->left == NULL)
  {
    bst_node_t * tmp = curr->right;
    
    //pokud jde o root
    if(prev == NULL)
    {
      (*tree)= tmp;
      free(curr);
      return;
    }
    
    //aktualizace ukazatelů na potomka mazaného uzlu
    if(curr == prev->left)
    {
      prev->left = tmp;
    }
    else
    {
      prev->right = tmp;
    }
    free(curr);
    return;
  }
  
  //uzel, který má 2 potomky
  else
  {
    bst_node_t tmp = *curr;
    bst_replace_by_rightmost(&tmp, &(tmp).left);
    curr->value = tmp.value;
    curr->key = tmp.key; 
    return;
  } 
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  
  //kontrola prázdného stromu
  if((*tree) == NULL)
  {
    return;
  }

  //vytvoření zásobníku
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if(stack == NULL)
  {
    return; //kontrola alokace
  }

  stack_bst_init(stack);
  stack_bst_push(stack, (*tree)); //vložení prvního prvku do zásobníku


  while (!stack_bst_empty(stack)) 
  {
      bst_node_t *tmp = stack_bst_pop(stack);
      if(tmp->left != NULL)
      {
        stack_bst_push(stack, tmp->left);
      } 
      if(tmp->right != NULL)
      {
        stack_bst_push(stack, tmp->right);
      } 
      (*tree) = tmp;
      free((*tree));
      (*tree) = NULL;
  }

  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
   while(tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }

}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  
  //vytvoření zásobníku
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if(stack == NULL)
  {
    return; //kontrola alokace
  }

  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack);

  while(!stack_bst_empty(stack))
  {
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack);
  }

  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  
  //vytvoření zásobníku
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if(stack == NULL)
  {
    return; //kontrola alokace
  }

  stack_bst_init(stack);
  bst_leftmost_inorder(tree, stack);

  while(!stack_bst_empty(stack))
  {
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, stack);
  }

  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  while(tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }

}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  
  //vytvoření zásobníku
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if(stack == NULL)
  {
    return; //kontrola alokace
  }

  stack_bool_t *stackBool = malloc(sizeof(stack_bool_t));
  if(stackBool == NULL)
  {
    return; //kontrola alokace
  }

  bool fromLeft; //rozlišuje průchod zleva nebo zprava

  stack_bst_init(stack);
  stack_bool_init(stackBool);
  bst_leftmost_postorder(tree, stack, stackBool);

  while(!stack_bst_empty(stack))
  {
    tree = stack_bst_top(stack);
    fromLeft = stack_bool_top(stackBool);
    stack_bool_pop(stackBool);
    if(fromLeft == true)
    {
      stack_bool_push(stackBool, false);//následující prvek jde zprava, tudíž fromLeft po nahrání ze stacku bude false
      bst_leftmost_postorder(tree->right, stack, stackBool);
    }
    else
    {
      stack_bst_pop(stack);
      bst_print_node(tree);
    }
  }

  free(stack);
  free(stackBool);
}
