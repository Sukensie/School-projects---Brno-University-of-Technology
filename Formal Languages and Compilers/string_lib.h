/*
Projekt do předmětů IFJ a IAL (ZS 2021)
Tým 031, varianta I

Autoři:
xploci01: Peter Pločica
xsouce15: Tomáš Souček
xcolog00: Adam Cologna
*/


#include <stdbool.h>

//struktura pro dynamicky retezec
typedef struct
{
	char *str; //retezec sam o sobe
	int strlen; //delka retezce
	int allocated_chars; //pro kolik znaku mame alokovanou pamet
} d_string; 

bool d_string_init(d_string *s);
void d_string_empty(d_string *s);
bool d_string_add_char(d_string *s, char c);
bool d_string_add_str(d_string *s, char *string_to_add);
bool d_string_transfer(d_string *source, d_string *destination);
void d_string_print(d_string *s);
