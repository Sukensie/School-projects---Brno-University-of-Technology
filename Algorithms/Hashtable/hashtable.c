/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

  for(int i=0; i < HT_SIZE; i++)
  {
    (*table)[i] = NULL; //stačí inicializovat jen index, nemusí jednotlivé části struktury
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

  for(int i = 0; i < HT_SIZE; i++)
  {
    if((*table)[i] != NULL && key  == (*table)[i]->key)
    {
      return (**table);
    }
  }
  
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  
  ht_item_t* searched = ht_search(table, key);
  if(searched != NULL)
  {
    searched->value = value;//nahrazení hodnoty v případě, že prvek již existuje
  }
  else
  {
    //vytvoření nového prvku
    ht_item_t* new = malloc(sizeof(ht_item_t));
    if(new == NULL)
    {
      return; //chyba alokace paměti
    }

    //nahrání hodnot do vkládaného prvku
    new->key = key;
    new->value = value;
    new->next = NULL;

    int index = get_hash(key);
    
    // Pokud k danému klíči existují synonyma, tak se zřetězí
    if ((searched = (*table)[index]))
    {
      new->next = searched;
    }

    (*table)[index] = new;// uložení nového prvku

  }

}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {

  ht_item_t* searched = ht_search(table, key);
  if(searched != NULL)
  {
   return &(searched)->value;//pokud prvek existuje v tabulce, vrátí adresu (&) jeho hodnoty
  }
  else
  {
    return NULL;
  }
 
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  
  int index = get_hash(key);
  ht_item_t* toBeDeleted = (*table)[index];
  ht_item_t* prev = NULL;

  //pokud mazaný prvek nemá synonyma, smaže se pouze on. Pokud nějaké má, díky cyklu se smažou postupně všechny 
  while(toBeDeleted != NULL)
  {
    //kontrola, jestli momentální prvek je ten, který chci mazat, nebo ještě musím loopovat skrz další položky indexu
    if(strcmp(toBeDeleted->key, key) == 0)
    {
      //nahrazení prvního prvku
      if(prev == NULL)
      {
        (*table)[index] = toBeDeleted->next;
        break;
      }

      prev->next = toBeDeleted->next; //zřetězení prvků    
      
      //uvolnení paměti mazaného prvku
      free(toBeDeleted);
      toBeDeleted = NULL;
      break;
      
    }
    prev = toBeDeleted; //stárnutí
    toBeDeleted = toBeDeleted->next; //posunutí na další položku indexu   
  }
 
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {

  for(int i = 0; i < HT_SIZE; i++)
  {
    ht_item_t* current = (*table)[i];

    //loop skrz synonyma
    while (current != NULL)
    {
      ht_item_t* help = current;
      current = current->next;
      free(help);
    }

    (*table)[i] = NULL; //nastavení celého indexu na NULL
  }
}
