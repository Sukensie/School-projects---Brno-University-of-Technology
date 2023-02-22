
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {

    //nastavím všechny prvky struktury na NULL, alokovat budu později
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    //probíhá postupné nahrazování a mazání prvního prvku, dokud nejsou všechny uvolněny
    DLLElementPtr help = list->firstElement; //potřebuji pomocnou proměnnou, abych si zapamatoval, který prvek chci smazat
    while(list->firstElement != NULL)
    {
        list->firstElement = list->firstElement->nextElement;
        free(help);
        help = list->firstElement;
    }
    
    //uvedení do stavu, ve kterém se nacházel po inicializaci
    DLL_Init(list);
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

   DLLElementPtr new = malloc(sizeof(struct DLLElement));
   if(new == NULL)
   {
       DLL_Error();
   }
   else
   {
       //nastavení dat, které platí vždy
        new->data = data;
        new->previousElement = NULL;
        new->nextElement = NULL;

        //pokud je list prázdný, pak vložený prvek bude zároveň první i poslední
        if(list->firstElement == NULL)
        {
            list->firstElement = new;
            list->lastElement = new;
        }
        else
        {
            //propojení prvního a vkládaného prvku
            list->firstElement->previousElement = new; 
            new->nextElement = list->firstElement;

            //nahrazení prvního prvku novým
            list->firstElement = new;
        }
   }
      
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

   DLLElementPtr new = malloc(sizeof(struct DLLElement));
   if(new == NULL)
   {
       DLL_Error();
   }
   else
   {
        new->data = data;
        new->nextElement = NULL;
    
        //pokud se list prázdný, volám funkci na vložení prvního prvku
        if(list->firstElement == NULL)
        {
            DLL_InsertFirst(list,data);
        }
        else
        {
            //propojení posledního a vkládaného prvku
            list->lastElement->nextElement = new;
            new->previousElement = list->lastElement;

            //nahrazení posledního prvku novým
            list->lastElement = new;
        }  
   }
}


/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    if(list->firstElement == NULL)
    {
        DLL_Error();
    }
    else
    {
        *dataPtr = list->firstElement->data;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    if(list->lastElement == NULL)
    {
        DLL_Error();
    }
    else
    {
        *dataPtr = list->lastElement->data;
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    //mazat první prvek, má smysl, pouze pokud existuje
    if(list->firstElement != NULL)
    {
        //pokud byl první prvek aktivní, aktivita se ztrácí
        if(list->firstElement == list->activeElement)
        {
            list->activeElement = NULL;
        }

        //když je v listu jen 1 prvek
        if(list->firstElement == list->lastElement)
        {
            DLL_Init(list); //pokud jsem odstranil poslední prvek, chci nastavit všechny 3 vlasnosti listu na NULL, neboli nově inicializovat
        }
        else
        {
            list->firstElement = list->firstElement->nextElement; //posunutí prvního prvku o 1 doprava
            list->firstElement->previousElement = NULL; //novému prvnímu prvku musím nastavit ukazatel na předchozí na NULL
        }
    }  
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    if(list->firstElement != NULL)
    {
        //pokud byl první prvek aktivní, aktivita se ztrácí
        if(list->lastElement == list->activeElement)
        {
            list->activeElement = NULL;
        }

        //když je v listu jen 1 prvek
        if(list->firstElement == list->lastElement)
        {
            DLL_Init(list); //pokud jsem odstranil poslední prvek, chci nastavit všechny 3 vlasnosti listu na NULL, neboli nově inicializovat
        }
        else
        {
            list->lastElement = list->lastElement->previousElement; //posunutí posledního prvku o 1 doleva
            list->lastElement->nextElement = NULL; //novému poslednímu prvku musím nastavit ukazatel na další na NULL
        }
    }
}
/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    
    //mazat má cenu pouze, pokud exituje aktivní prvek a zároveň aktivní prvek není posledním prvek listu
    if(list->activeElement != NULL && list->activeElement != list->lastElement)
    {
        //pomocná proměnná pro uchování prvku následujícím po aktivním
        DLLElementPtr next;
        next = list->activeElement->nextElement;
        
        list->activeElement->nextElement = next->nextElement; //nastaví nové propojení aktivního prvku s následovným, nebo pokud je mazaný prvek poslední, nastaví následovný prvek na NULL

        if(next->nextElement != NULL)
        {
            next->nextElement->previousElement = list->activeElement;
        }
        else
        {
            list->lastElement = list->activeElement;
        }
        free(next);//následné uvolnení alokované paměti
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if(list->activeElement != NULL && list->activeElement != list->firstElement)
    {
        //pomocná proměnná pro uchování prvku předcházejícím aktivnímu
        DLLElementPtr previous;
        previous = list->activeElement->previousElement;
        
        list->activeElement->previousElement = previous->previousElement;//obodobný případ jako u DLL_DeleteAfter()
        if(previous->previousElement != NULL)
        {
            previous->previousElement->nextElement = list->activeElement;
        }
        else
        {
            list->firstElement = list->activeElement;
        }
        free(previous);//následné uvolnení alokované paměti
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    if(list->activeElement != NULL)
    {
        DLLElementPtr new = malloc(sizeof(struct DLLElement));
        if(new == NULL)
        {
            DLL_Error();
        }
        else
        {
            //pokud je aktivní prvek poslední, můžu zavolat funkci na vložení posledního prvku
            if(list->activeElement == list->lastElement)
            {
                DLL_InsertLast(list,data);
            }
            else
            {
                //pokud je aktivní prvek první, můžu zavolat funkci na vložení prvního prvku
                new->data = data;
                new->previousElement = list->activeElement;
                new->nextElement = list->activeElement->nextElement;

                //propojení současných prvků s novým
                list->activeElement->nextElement = new;
                list->activeElement->nextElement->previousElement = new;
            }
            free(new);//následné uvolnení alokované paměti
        }
        
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    
    if(list->activeElement != NULL)
    {
        DLLElementPtr new = malloc(sizeof(struct DLLElement));
        if(new == NULL)
        {
            DLL_Error();
        }
        else
        {
            //pokud je aktivní prvek první, můžu zavolat funkci na vložení prvního prvku
            if(list->activeElement == list->firstElement)
            {
                DLL_InsertFirst(list,data);
            }
            else
            {
                //propojení nového prvku
                new->data = data;
                new->nextElement = list->activeElement;
                new->previousElement = list->activeElement->previousElement;

                //propojení současných prvků s novým
                list->activeElement->previousElement->nextElement = new;
                list->activeElement->previousElement = new;
            }
            free(new);//následné uvolnení alokované paměti
        }
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    if(list->activeElement == NULL)
    {
        DLL_Error();
    }
    else
    {
        *dataPtr = list->activeElement->data;
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    if(list->activeElement != NULL)
    {
        list->activeElement->data = data;
    }
  
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    if(list->activeElement != NULL)
    {
        list->activeElement = list->activeElement->nextElement;
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    if(list->activeElement != NULL)
    {
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {

    return list->activeElement != NULL ? 1 : 0;
   
}

/* Konec c206.c */
