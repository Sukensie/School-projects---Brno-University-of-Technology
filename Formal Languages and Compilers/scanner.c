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
#include "errors.h"

int toBeAlocated;


Token_t initToken()
{
    Token_t token;

    toBeAlocated = 20;

    token.type = 0; //0...type=start
    token.attribute = malloc(toBeAlocated*sizeof(char));
    token.length = 0;

    if(token.attribute == NULL)
    {
        call_error(INTERNAL_ERROR);
    }

    return token;
}

//token je jako ukazatel, aby fungovala inkrementace length
void insertAttributeChar(Token_t* token, char inserted)
{
   
    if(token->length+2> toBeAlocated) //+2 protože vkládám char a \0
    {
        toBeAlocated = toBeAlocated * 2;
        token->attribute = (char*) realloc(token->attribute, (toBeAlocated)*sizeof(char));
        if(token->attribute == NULL)
        {
           call_error(INTERNAL_ERROR);
        }
    }
    token->attribute[token->length] = inserted;
    token->attribute[token->length+1] = '\0';//musím na konec vložit znak konce řetězce, jinak bude házet nesmyslné výsledky
    token->length++;
}
void deleteLastAttributeChar(Token_t* token)
{
    if(token->length > 0)
    {
        token->attribute[token->length-1] = '\0';
        token->length--;
    }
    else
    {
        call_error(INTERNAL_ERROR);
    }
   
}

void destroyToken(Token_t token)
{
    free(token.attribute);
}

void printToken(Token_t token)
{
    fprintf(stdout,"type = '");
    switch(token.type)
    {
        case 0:
            fprintf(stdout,"start");
            break;
        
        case 1:
            fprintf(stdout,"error");
            break;
        
        case 2:
            fprintf(stdout,"id");
            break;
        
         case 3:
            fprintf(stdout,"=");
            break;

        case 4:
            fprintf(stdout,"number");
            break;
        
        case 5:
            fprintf(stdout,",");
            break;
         
        case 6:
            fprintf(stdout,"*");
            break;
        
        case 7:
            fprintf(stdout,"/");
            break;

        case 8:
            fprintf(stdout,"+");
            break;
        
        case 9:
            fprintf(stdout,"-");
            break;

        case 10:
            fprintf(stdout,"(");
            break;

        case 11:
            fprintf(stdout,")");
            break;

        case 12:
            fprintf(stdout,"~");
            break;

        case 13:
            fprintf(stdout,".");
            break;

        case 14:
            fprintf(stdout,":");
            break;
        
        case 15:
            fprintf(stdout,">");
            break;

        case 16:
            fprintf(stdout,"<");
            break;
        
        case 17:
            fprintf(stdout,"#");
            break;

        case 18:
        case 19:
        case 20:
            fprintf(stdout,"string");
            break;
        //skip 19,20 escapes    
        case 21:
        case 22:
        case 23:
            fprintf(stdout,"comment");
            break;

        case 24:
            fprintf(stdout,">=");
            break;
            
        case 25:
            fprintf(stdout,"<=");
            break;
        
        case 26:
            fprintf(stdout,"..");
            break;

        case 27:
            fprintf(stdout,"~=");
            break;

        case 28:
            fprintf(stdout,"==");
            break;

        case 29:
            fprintf(stdout,"//");
            break;

        case 32:
            fprintf(stdout, "keywordDo");
            break;

        case 33:
            fprintf(stdout, "keywordElse");
            break;

        case 34:
            fprintf(stdout, "keywordEnd");
            break;

        case 35:
            fprintf(stdout, "keywordFunction");
            break;

        case 36:
            fprintf(stdout, "keywordGlobal");
            break;

        case 37:
            fprintf(stdout, "keywordIf");
            break;

        case 38:
            fprintf(stdout, "keywordInteger");
            break;

        case 39:
            fprintf(stdout, "keywordLocal");
            break;

        case 40:
            fprintf(stdout, "keywordNil");
            break;

        case 41:
            fprintf(stdout, "keywordNumber");
            break;

        case 42:
            fprintf(stdout, "keywordRequire");
            break;

        case 43:
            fprintf(stdout, "keywordReturn");

            break;
        case 44:
            fprintf(stdout, "keywordString");
            break;

        case 45:
            fprintf(stdout, "keywordThen");
            break;
            
        case 46:
            fprintf(stdout, "keywordWhile");
            break;  

        case 47:
            fprintf(stdout, "integer");
            break;        
    }
    fprintf(stdout,"' | attribute = '%s'\n", token.attribute);
}
/**
*   Vrací téměř hotový token, akorát je ještě potřeba projít ID tokeny a nevyhovující opravit na KEYWORDS
**/
Token_t getDraftToken()
{
    char c;
    Token_t token = initToken();
    State_t state = start;

    //pokud už je input na konci, vrátí automat do počátečního stavu
    if((c = fgetc(stdin)) == EOF)
    {
        token.type = start;
        return token;
    }
    else
    {
        ungetc(c, stdin); //abych nepřišel o první znak
    }

    while((c = fgetc(stdin)) != EOF)
    {
        switch (state)
        {
            case start:
                if(c == EOF || isspace(c))
                {                   
                    state = start;
                    break;
                }
                if(isalpha(c) || c == '_')
                {
                    state = id;
                    insertAttributeChar(&token,c);
                    break;
                }
                if(c == '=')
                {
                    state=equals;
                    break;
                }
                if(c == ',')
                {
                    token.type = comma;
                    state = start;
                    return token;
                    break;
                }
                if(c == '*')
                {
                    token.type = multiply;
                    state = start;
                    return token;
                    break;
                }
                if(c == '/')
                {
                    state=divide;
                    break;
                }
                if(c == '+')
                {
                    token.type = plus;
                    state = start;
                    return token;
                    break;
                }
                if(c == '-')
                {
                    state=minus;
                    break;
                }
                if(c == '(')
                {
                    token.type = lBracket;
                    state = start;
                    return token;
                    break;
                }
                if(c == ')')
                {
                    token.type = rBracket;
                    state = start;
                    return token;
                    break;
                }
                if(c == '~')
                {
                    state=not;
                    break;
                }
                if(c == '.')
                {
                    state=dot;
                    break;
                }
                if(c == ':')
                {
                    token.type = colon;
                    state = start;
                    return token;
                    break;
                }
                if(c == '>')
                {
                    state=greater;
                    break;
                }
                if(c == '<')
                {
                    state=lower;
                    break;
                }
                if(c == '#')
                {
                    token.type = hash;
                    state = start;
                    return token;
                    break;
                }
                if(isdigit(c))
                {
                    state = number;
                    insertAttributeChar(&token,c);
                    break;
                }
                if(c == '"')
                {
                    state = string;
                    break;
                }
                call_error(LEX_ERROR);
                break;
            
            case id :
                
                if(isalpha(c) || isdigit(c) || c == '_')
                {
                    state = id;
                    insertAttributeChar(&token, c);
                    break;
                }
                else
                {
                    ungetc(c, stdin);
                }
                token.type = state;
                state = start;
                return token;
                break;

            case equals:
                if(c == '=')
                {
                    token.type = doesEqual;    
                    state = start;
                    return token;
                }
                else
                {
                    ungetc(c, stdin);
                }
                token.type = state;    
                state = start;
                return token;
                break;

            case number:
                if(isspace(c) || c == EOF)
                {
                    token.type = integer;
                    state = start;
                    return token;
                }
                if(isdigit(c))
                {
                    state = number;
                    insertAttributeChar(&token, c);
                    break;
                }
                if(c == '.')
                {
                    state = numberDot;
                    insertAttributeChar(&token, c);
                    break;
                }
                if(c == 'e' || c == 'E')
                {
                    state = numberExp;
                    insertAttributeChar(&token, c);
                    break;
                }
                else
                {
                    ungetc(c, stdin);
                    token.type = integer;
                    state = start;
                    return token;
                }
                break;
            
            case numberDot:
                if(isdigit(c))
                {
                   insertAttributeChar(&token, c);
                   break;
                }
                else if((c == 'e' || c == 'E') && (token.length > 0 && isdigit(token.attribute[token.length-1])))
                {
                    state = numberExp;
                    insertAttributeChar(&token, c);
                    break;
                }
                else if ((isspace(c) && token.length > 0 && isdigit(token.attribute[token.length-1])) || c == '(' || c == ')' || c == ',')//kontrola alespoň 1 čísla za tečkou
                {
                    ungetc(c,stdin); //PP
                    token.type = number;    
                    state = start;
                    return token;
                }
                else
                {
                    call_error(LEX_ERROR);
                }
                break;
            
            case numberExp:
                if(isdigit(c) || c == '+' || c == '-')
                { 
                   insertAttributeChar(&token, c);
                   break;
                }
                else if ((isspace(c) && token.length > 0 && isdigit(token.attribute[token.length-1])) || c == '(' || c == ')' || c == ',')
                {
                    ungetc(c, stdin); //PP
                    token.type = number;    
                    state = start;
                    return token;
                }
                else
                {
                    call_error(LEX_ERROR);
                }
                break;

            case divide:
                if(c == '/')
                {
                    token.type = intDivide;    
                    state = start;
                    return token;
                }
                else
                {
                    ungetc(c, stdin);
                }
                token.type = state;
                state = start;
                return token;
                break;
           
            case minus:
                if(c == '-')
                {
                    state = comment;
                    break;
                }
                else
                {
                    ungetc(c, stdin);
                }
                token.type = state;
                state = start;
                return token;
                break;
            
            case comment:
                //v tuhle chvíli mám --c
               
                if(c == '[')
                {
                    char d = fgetc(stdin);//potřebuju znát další znak... bud blokový koment nebo line
                    if(c == '[' && d == '[' )
                    {
                        state = blockComment;
                    }
                  
                    break;
                }
                
                insertAttributeChar(&token,c);
                state = lineComment;
                break;

            case lineComment:    
                if(c == '\n' || c == EOF)
                {
                    token.type = comment;
                    state = start;
                    return token;
                }
                insertAttributeChar(&token, c);
                break;

            case blockComment:
                if(c == ']' && token.attribute[token.length-1] == ']')
                {   
                    deleteLastAttributeChar(&token);
                    token.type = comment;
                    state = start;
                    return token;
                }
                insertAttributeChar(&token, c);
                break;
            
            case not:
                if(c == '=')
                {
                    token.type = notEqual;
                    state = start;
                    return token;
                }
                else
                {
                   call_error(LEX_ERROR);
                }
                break;
            
            case dot:
                if(c == '.')
                {
                    token.type = concatenate;
                    state = start;
                    return token;
                }
                else
                {
                    call_error(LEX_ERROR);
                }
                break;

            case greater:
                if(c == '=')
                {
                    token.type = greaterEqual;
                    state = start;
                    return token;
                }
                else
                {
                    ungetc(c, stdin);
                }
                token.type = state;
                state = start;
                return token;
                break;

            case lower:
                if(c == '=')
                {
                    token.type = lowerEqual;
                    state = start;
                    return token;
                }
                else
                {
                    ungetc(c, stdin);
                }
                token.type = state;
                state = start;
                return token;
                break;

            case string:
                if(c == '"')
                {
                    token.type = state;
                    state = start;
                    return token;
                }

                if(c == 92)//lomeno
                {
                    insertAttributeChar(&token, c);
                    state=stringEscape;
                    break;
                }
                else if(c <= 32 || c == 35)
                {
                    char string[4];
                    sprintf(string, "%03d", c);//přemění int na string a pokud není tříčíselný, doplní nuly na začátek

                    insertAttributeChar(&token, '\\');
                    insertAttributeChar(&token, string[0]);
                    insertAttributeChar(&token, string[1]);
                    insertAttributeChar(&token, string[2]);
                }             
                else
                {
                   insertAttributeChar(&token, c);
                }
               
                break;

            case stringEscape:
                //ošetření whitespace znaků
                if(c == 't')
                {
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '9');
                    state = string;
                    break;
                }
                if(c == 'n')
                {
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '1');
                    insertAttributeChar(&token, '0');
                    state = string;
                    break;
                }
                if(c == 'v')
                {
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '1');
                    insertAttributeChar(&token, '1');
                    state = string;
                    break;
                }
                if(c == 'f')
                {
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '1');
                    insertAttributeChar(&token, '2');
                    state = string;
                    break;
                }
                if(c == 'r')
                {
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '1');
                    insertAttributeChar(&token, '3');
                    state = string;
                    break;
                }

                //osamocené lomeno
                if(c ==' ')
                {
                    //lomeno
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '9');
                    insertAttributeChar(&token, '2');

                    //mezera za lomenem
                    insertAttributeChar(&token, '\\');
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '3');
                    insertAttributeChar(&token, '2');
                    state = string;
                    break; 
                }

                //lomeno + lomeno
                if(c == 92)
                {
                    insertAttributeChar(&token, '0');
                    insertAttributeChar(&token, '9');
                    insertAttributeChar(&token, '2');
                    state = string;
                    break; 
                }

                // lomeno + číslo
                if(isdigit(c))
                {
                    state = stringNumEscape;
                    insertAttributeChar(&token, c);//potřebuju si to číslo poslat, abych znal všechna 3 escape čísla
                    break;
                }

                //lomeno + tisknutelný znak
                deleteLastAttributeChar(&token);//odstraní přebytečné lomeno
                insertAttributeChar(&token, c);
                state = string;
                break;

            case stringNumEscape:
            {
                //u case musí být {}, abych mohl definovat pole hned na začátku

                char array [4];
                array[0] = token.attribute[token.length-1];
                deleteLastAttributeChar(&token);//nechci první číslo vypsat, tak ho odstarním

                int pivot = 1;
                //začínám nahrávat od 1, protože první znak vkládám speciálně
                for(int i = 1; i < 3; i++)
                {
                    if(isdigit(c))
                    {
                        array[pivot] = c;
                        pivot++;
                    }
                    else
                    {
                        call_error(LEX_ERROR);//vím, že první znak je číslo, tím pádem pokud nedostanu další číslo, jedná se o špatnou escape sekvenci
                    }
                    
                    if(i+1 < 3)
                    {
                        c = fgetc(stdin);
                    }
                } 
               

                //pokud pole obsahuje 3 čísla
                if(pivot == 3)
                {
                    int res = atoi(array);
                    if(!(res >= 0 && res <=255)) //kontrola intervalu
                    {
                        call_error(LEX_ERROR);
                    }
                    else
                    {
                        insertAttributeChar(&token, array[0]);
                        insertAttributeChar(&token, array[1]);
                        insertAttributeChar(&token, array[2]);
                    }
                    state = string;
                }
                else
                {
                    call_error(LEX_ERROR);
                }
                
                break;
            }

            default:
                break;
        }           
    }

    token.type = state;
    return token;
    
}

/**
 * Pokud se atribut tokenu typu ID rovná nějakému klíčovému slovu, převede typ tokenu z ID na KEYWORD
 **/
Token_t idToKeyword(Token_t token)
{
    if(token.type == id)
    {   
        if(strcmp(token.attribute, "do") == 0)
        {
            token.type = keywordDo;
        }
        if(strcmp(token.attribute, "else") == 0)
        {
            token.type = keywordElse;
        }
        if(strcmp(token.attribute, "end") == 0)
        {
            token.type = keywordEnd;
        }
        if(strcmp(token.attribute, "function") == 0)
        {
            token.type = keywordFunction;
        }
        if(strcmp(token.attribute, "global") == 0)
        {
            token.type = keywordGlobal;
        }
        if(strcmp(token.attribute, "if") == 0 )
        {
            token.type = keywordIf;
        }
        if(strcmp(token.attribute, "integer") == 0 )
        {
            token.type = keywordInteger;
        }
        if(strcmp(token.attribute, "local") == 0)
        {
            token.type = keywordLocal;
        }
        if(strcmp(token.attribute, "nil") == 0)
        {
            token.type = keywordNil;
        }
        if(strcmp(token.attribute, "number") == 0)
        {
            token.type = keywordNumber;
        }
        if(strcmp(token.attribute, "require") == 0)
        {
            token.type = keywordRequire;
        }
        if(strcmp(token.attribute, "return") == 0)
        {
            token.type = keywordReturn;
        }
        if(strcmp(token.attribute, "string") == 0)
        {
            token.type = keywordString;
        }
        if(strcmp(token.attribute, "then") == 0)
        {
            token.type = keywordThen;
        }
        if(strcmp(token.attribute, "while") == 0)
        {
            token.type = keywordWhile;
        }
    }
    return token;

}

Token_t getToken()
{
    Token_t token;
    token = getDraftToken();
    token = idToKeyword(token);

    //komentář nechci posílat parseru -> zažádám o nový token
    if(token.type == lineComment || token.type == blockComment || token.type == comment)
    {
        if(token.type == blockComment && token.attribute[token.length-1] != ']')//chyba neukočeného komentáře
        {
            call_error(LEX_ERROR);
        }
        token = getToken();
    }

    return token;
}

/*int main()
{   
    Token_t token;
    while(1)
    {   
        token = getToken();
        if(token.type == start)
        {
            break;
        }
        printToken(token);
        destroyToken(token);
    }
    
    
    return 1;
}*/

