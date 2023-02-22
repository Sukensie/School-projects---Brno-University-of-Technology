
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    while(!Stack_IsEmpty(stack))
    {
        char c = '`'; //incializace znakem, který se nevyskytuje v množině možných vstupů
        Stack_Top(stack,&c); //načte operátor z vrcholu zásobníku do c
        Stack_Pop(stack); //odstraní operátor z vrcholu zásobníku

        //pokud narazím na levou závorku, splnil jsem cílovou podmínku funkce a chci tedy její výkon ukončit
        if(c == '(')
        {
            break;
        }
        
        postfixExpression[*postfixExpressionLength] = c; //nahraje operátor do výstupu
        (*postfixExpressionLength)++;
          
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    char topChar = '`';//inicializuji pro znak, který se v inputu nevyskytuje

    //když vstoupím do doOperation, vím jistě, že mám operand
    
    //kontrola, abych nepožadoval znak z prázdného zásobníku. Podmínka by nemusela nutně být, protože v Stack_Top() se kontroluje to samé, ale mám ji tam, aby mi to v testech napsalo několikrát Stack_Error()
    if(!Stack_IsEmpty(stack))
    {
        Stack_Top(stack,&topChar);
    }
    
    //pokud je to operand splnujici podminky definované v popisu algoritmu (studijní opora), pushnu ho na stack. Jinak ho nahraji do výsledku a zavolám funkci doOperation() znovu 
    if(Stack_IsEmpty(stack) || topChar == '(' || ((c == '/' || c == '*') && (topChar == '+' || topChar == '-')))
    {   
        Stack_Push(stack, c);
    }
    else
    {
        postfixExpression[*postfixExpressionLength] = topChar;
        (*postfixExpressionLength)++;
        Stack_Pop(stack);
        doOperation(stack, c, postfixExpression, postfixExpressionLength);
    }  

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {

    //kontrola alokace paměti
    Stack * stack = malloc(sizeof(Stack));
    if(stack == NULL)
    {
        return NULL;
    }
    Stack_Init(stack); //inicializace až po kontrole, zda se podařilo alokovat paměť

    //kontrola alokace paměti
    char * result = malloc(MAX_LEN * sizeof(char));
    if(result == NULL)
    {
        return NULL;
    }


    int i = 0; //index pro cteni
    unsigned int pivot = 0; //index pro zapis
    char c = infixExpression[i]; //čtecí char
    char topChar = '`'; //char vyskytující se na vrcholu zásobníku

    //čtení zleva doprava... buď dojede na konec řetězce, nebo je omezen maximální definovanou délkou
    while(c != '\0' && i < MAX_LEN)
    {
        //postupně procházím jednotlivé kroky popsané v algoritmu ve studijní opoře

        //pokud je daný výraz operand (znak nebo číslo), přidá se na konec výsledku
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        {
            //uložení znaku
            result[pivot] = c;
            pivot++;
        }
        else if(c == '(') //krok 3...Je-li zpracovávanou položkou levá závorka, vlož ji na vrchol zásobníku.
        {
            Stack_Push(stack,c);
        }
        else if(c == ')') //krok 6...Je-li zpracovávanou položkou pravá závorka, odebírej z vrcholu položky a dávej je na konec výstupního řetězce až narazíš na levou závorku. Tím je pár závorek zpracován
        {
            untilLeftPar(stack,result,&pivot);  
        }
        else if(c == '=')
        {
            while(!Stack_IsEmpty(stack))
            {
                Stack_Top(stack,&topChar);
                Stack_Pop(stack);
                result[pivot] = topChar;
                pivot++;
            }
            result[pivot] = c;
            pivot++;
        } 
        else
        {
            doOperation(stack, c, result, &pivot);
        }

    
        //získání dalšího znaku
        i++;
        c = infixExpression[i];
    }


    free(stack);    //stack by měl být v tuto chvíli prázdný a tak jej můžu uvolnit bez ztráty potřebných dat
    result[pivot] = '\0';//nutné pro validní ukončení řetězce
    return result;
}

/* Konec c204.c */
