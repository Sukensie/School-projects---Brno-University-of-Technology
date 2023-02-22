/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  
  //pokud je strom prázdný
  if(tree == NULL)
  {
    return false;
  }

  if(tree->key == key)
  {
    *value = tree->value;
    return true;
  }
  
  if (key < tree->key) //levá větev
  {
    return bst_search(tree->left, key, value);
  }
  else //pravá větev
  {
    return bst_search(tree->right, key, value);
  }
  
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {


  //vložení listu
  if((*tree) == NULL)
  {
    (*tree) = (bst_node_t*) malloc(sizeof(bst_node_t));
    if((*tree) == NULL)
    {
      return; //nepovedená alokace
    }
    
    (*tree)->value = value;
    (*tree)->key = key;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
  }
 


  //nahrazení uzlu se stejným klíčem
  if(key == (*tree)->key)
  {
    (*tree)->value = value;
    return;
  }

  if(key < (*tree)->key)//levá větev
  {
		bst_insert(&((*tree)->left), key, value);
  }
  else //pravá větev
  {
    bst_insert(&((*tree)->right), key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  //pokud stále existuje nějaký pravý list, zavolám funkci znovu pro pravou větev uzlu
  if((*tree)->right != NULL)
  {
    bst_replace_by_rightmost(target, &(*tree)->right);
  }
  else
  {
    //nahrazení hodnot  
    target->key = (*tree)->key;
    target->value = (*tree)->value;

    //nahrání levého listu. Pravý nemá smysl, protože v téhle fázi jsem v nejpravějším listu --TODO TAHLE FUNKCE
   (*tree) = (*tree)->left;   
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
   
  //pokud je strom prázdný, nestane se nic
  if((*tree) == NULL)
  {
    return;
  } 

  //hledání uzlu
  if(key < (*tree)->key)//levá větev
  {
		bst_delete(&((*tree)->left), key);
  }

  if(key > (*tree)->key)//pravá větev
  {
		bst_delete(&((*tree)->right), key);
  }

  //nalezení uzlu
  if(key == (*tree)->key)
  {
    
    //uzel, který nemá potomky
    if((*tree)->left == NULL && (*tree)->right == NULL)
    {
      free((*tree));
      (*tree) = NULL;
      return;
    }

    //uzel, který má pouze levého potomka
    else if((*tree)->right == NULL)
    {
      bst_node_t * tmp = (*tree)->left;
      free((*tree));
      (*tree) = tmp;
    }

    //uzel, který má pouze pravého potomka
    else if((*tree)->left == NULL)
    {
      bst_node_t * tmp = (*tree)->right;
      free((*tree));
      (*tree) = tmp;
    }

    //uzel, který má oba potomky
    else
    {  
      bst_node_t* tmp = (*tree);
      bst_replace_by_rightmost(tmp, &(tmp)->left);      
      (*tree) = tmp;
    }
  }
}


/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  
  if((*tree) != NULL)
  {
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);

    free((*tree));
    (*tree) = NULL;
  }
  
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  
  if(tree != NULL)
  {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {

  if(tree != NULL)
  {
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
 
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  
 if(tree != NULL)
  {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
