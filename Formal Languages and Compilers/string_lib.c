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

#include "string_lib.h"

int d_string_basic_strlen = 16; //pocet charu, ktere budeme bezne inicializovat

void d_string_empty(d_string *s){ //smazani obsahu dynamickeho retezce
	s->strlen = 0; //nastavime delku dynamickeho retezce na 0
	s->str[s->strlen] = '\0'; //a na prvni (nulty) prvek vlozime znak konce retezce
}


bool d_string_init(d_string *s){ //inicializace retezce
	s->str = (char *) malloc(d_string_basic_strlen); //10 znaků
	if (!(s->str)){ //overime, ze malloc uspesne probehl
		return false;
	}

	s->allocated_chars = d_string_basic_strlen; //do poctu prirazenych znaku pridame pocet znaku, ktery jsme alokovali pomoci funkce malloc

	return true;
}

bool d_string_add_char(d_string *s, char c){
	if (s->strlen > s->allocated_chars){ //pokud jiz neni dostatek alokovane pameti
		//tak ji pridame vice
		int add_mem = s->strlen + d_string_basic_strlen; 
		s->str = (char *) realloc(s->str, add_mem);
		if (!s->str){ //pokud se nepodaril realloc
			return false;
		}
		s->allocated_chars = add_mem;
	}
	//tahle cast probehne v kazdem pripade
	s->str[s->strlen] = c; //za posledni znak pripiseme znak c
	s->strlen++; 
	s->str[s->strlen] = '\0'; //uplne na konec retezce prijde ukonceni

	return true;
}

bool d_string_transfer(d_string *source, d_string *destination){
	if (source->strlen >= destination->allocated_chars){ //pokud je v cilovem retezci dostatek mista pro puvodni retezec
		destination->str = (char *) realloc(destination->str, source->strlen + 1);
		if (!destination->str){ //tak overime ze se povedl realloc
			return false;
		}
		destination->allocated_chars = source->strlen + 1; //musime pricist kvuli ukoncovacimu znaku
	}

	strcpy(destination->str, source->str); //pokud vse probehlo ok, tak prekopirujeme puvodni retezec do ciloveho
	destination->strlen = source->strlen; //i delku priradime spravne

	return true;
}
//pridelani retezce do dynamickeho
bool d_string_add_str(d_string *s, char *string_to_add){
	int strlen_string_to_add = strlen(string_to_add); //zjistime delku obdrzeneho retezce

	if (s->strlen + strlen_string_to_add + 1 >= s->allocated_chars){ //overime jestli musime provadet realokaci pameti
		int add_mem = s->strlen + strlen_string_to_add + 1;
		s->str = (char *) realloc(s->str, add_mem);
		if (!s->str){
			return false;
		}
		s->allocated_chars = add_mem;
	}

	s->strlen = s->strlen + strlen_string_to_add; //prictemu delku retezce k pripsani
	strcat(s->str, string_to_add); //provedem konkatenaci dvou retezcu

	return true;
}


void d_string_print(d_string *s){ //jednoducha funkce na vypsani retezce
	printf("%s\n", s->str);
}