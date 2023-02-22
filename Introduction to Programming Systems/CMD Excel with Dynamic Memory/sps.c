#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


typedef struct
{
    unsigned int length;
    char * data;

}cell_t;

typedef struct
{
    unsigned int numCells;
    cell_t * cells;

}row_t;

typedef struct
{
    unsigned int numRows;
    row_t * rows;

}table_t;


typedef struct
{
    unsigned int length;
    char * data;

}command_t;

typedef struct
{
    unsigned int numCommands;
    command_t * prikazy;

}commandAray_t;

typedef struct
{
    int r1;
    int r2;
    int c1;
    int c2;
}selection_t;

typedef struct
{
    unsigned int length;
    char * data;

}tmp_t;

typedef struct
{
    unsigned int numTmps;
    tmp_t tmps [10];

}tmpArray_t;

void addCharToTmp(tmp_t * tmp, char znak)
{
    tmp->length++;
    if(tmp->length == 1)
    {
       tmp->data = malloc(sizeof(char));
    }
    else
    {
        tmp->data = realloc(tmp->data ,tmp->length * sizeof(char));
    }
    tmp->data[tmp->length-1] = znak;
}

void addCharToCell(cell_t * bunka, char znak)
{
    bunka->length++;
    if(bunka->length == 1)
    {
       bunka->data = malloc(sizeof(char));
    }
    else
    {
        bunka->data = realloc(bunka->data ,bunka->length * sizeof(char));
    }
    bunka->data[bunka->length-1] = znak;
}

void addCellToRow (row_t * radek, cell_t bunka)
{
    radek->numCells++;
    if(radek->numCells == 1)
    {
        radek->cells = malloc(sizeof(cell_t));
    }
    else
    {
        radek->cells = realloc( radek->cells, radek->numCells * sizeof(cell_t));
    }
    radek->cells[radek->numCells-1] = bunka;
}

void addRowToTable(table_t * tabulka, row_t radek)
{
    tabulka->numRows++;
    if(tabulka->numRows == 1)
    {
        tabulka->rows = malloc(sizeof(row_t));
    }
    else
    {
        tabulka->rows = realloc(tabulka->rows, tabulka->numRows * sizeof(row_t));
    }
    tabulka->rows[tabulka->numRows-1] = radek;
}


void addCharToCommand(command_t * prikaz, char znak)
{
    prikaz->length++;
    if(prikaz->length == 1)
    {
       prikaz->data = malloc(sizeof(char));
    }
    else
    {
        prikaz->data = realloc(prikaz->data ,prikaz->length * sizeof(char));
    }
    prikaz->data[prikaz->length-1] = znak;
}

void addCommandToArray (commandAray_t * polePrikazu, command_t prikaz)
{
    polePrikazu->numCommands++;
    if(polePrikazu->numCommands == 1)
    {
        polePrikazu->prikazy = malloc(sizeof(command_t));
    }
    else
    {
        polePrikazu->prikazy = realloc( polePrikazu->prikazy, polePrikazu->numCommands * sizeof(command_t));
    }
    polePrikazu->prikazy[polePrikazu->numCommands-1] = prikaz;
}

int isDelim(char pismeno, char * delim)
{
    int wasDelim = 0;
    int delimLength =  strlen(delim);

    for(int i=0; i < delimLength; i++)
    {
        if(pismeno == delim[i])
        {
            wasDelim++;
            break;
        }
    }
    if(wasDelim > 0)
    {
        return 1;
    }
    else
    {
       return 0;
    }

}

int countFloat(float number)
{
    char help[14];
    sprintf(help,"%g",number);

    return strlen(help);
}

void vypisTabulku (table_t tabulka, FILE * soubor, char * delim)
{
    bool printDelim;
    int pocetRadku = tabulka.numRows;
    int pocetSloupcu = tabulka.rows->numCells;
    for(int i =0; i < pocetRadku; i++)
    {
        printDelim = false;
        for(int j = 0; j < pocetSloupcu; j++)
        {
            if(printDelim == true)
            {
                fprintf(soubor,"%c", delim[0]);
            }
              fprintf(soubor,"%s", tabulka.rows[i].cells[j].data);
              printDelim=true;
        }
        fprintf(soubor,"\n");
    }
}
void inicializujSelekci(selection_t * selekce)
{
    selekce->r1 = 1;
    selekce->r2 = 1;
    selekce->c1 = 1;
    selekce->c2 = 1;
}
int substrPositon(commandAray_t polePrikazu, int q)
{
   int pozice = 1;

   while(polePrikazu.prikazy[q].data[pozice-1] != ' ')
   {
        pozice++;
   }
   return pozice;
}

int substringLenght(commandAray_t polePrikazu, int q)
{
    int pozice = substrPositon(polePrikazu, q);
    int delkaPrikazu = polePrikazu.prikazy[q].length;
    int rozdil = delkaPrikazu - pozice +1;

    return rozdil;
}


void substring(commandAray_t polePrikazu, int q, char pole[])
{
    int pozice = substrPositon(polePrikazu,q);
    int rozdil = substringLenght(polePrikazu,q);

    for(int i = 0; i < rozdil-1;i++)
    {
        pole[i] = polePrikazu.prikazy[q].data[pozice+i];
    }
     pole[rozdil-1] = '\0';

}

int getCordinates(char * str, long * poleSouradnic)
{
    //inicializace poleSouradnic
    for(int i = 0; i < 4; i++)
    {
         poleSouradnic[i] = 0;
    }

    int i = 0;
    char *p = str;// do *p se nahraje konrkrétní jeden příkaz
    while (*p)
    {
        if (isdigit(*p))
        {
            //isDigit, protože chci odchyntout jen cela cisla
            poleSouradnic[i] = strtol(p, &p, 10);
            i++;
        }
        else
        {
            if(*p == '_' || *p == '-')
            {
                poleSouradnic[i] = -1;
                i++;
            }
            p++;//posun se na dalsi pismeno
        }
    }
    return i;//vrati pocet souradnic
}

int isSelectedCell(selection_t selekce)
{
    if(selekce.r1 == selekce.r2 && selekce.c1 == selekce.c2)
    {
        return 1;
    }
    return -1;
}

void replaceCellWithNumber(float number, table_t tabulka, long * poleSouradnic)
{
      int pocetCisel = countFloat(number);

      tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data = realloc(tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data, (pocetCisel * sizeof(char))+1);
      sprintf(tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data,"%g", number);
      tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].length = strlen(tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data) +1;
}

int countEmptyCells(table_t tabulka, int i, int j, int * count)
{
     if(strcmp(tabulka.rows[i].cells[j].data, "\0") == 0)
     {
         *count = *count + 1;
     }
    return *count;
}

int isFloat(table_t tabulka, int i, int j)
{
    //za float povaĹžuji vstup kterĂ˝ obsahuje buÄ ÄĂ­slice nebo teÄku
    int spravne = 0;
    int delka = tabulka.rows[i].cells[j].length;
    for(int q =0; q < delka-1;q++)
    {
        if((tabulka.rows[i].cells[j].data[q] >= '0' && tabulka.rows[i].cells[j].data[q] <= '9' ) || tabulka.rows[i].cells[j].data[q] == '.' || tabulka.rows[i].cells[j].data[q] == '-')
        {
            spravne++;
        }
        else
        {
           return -1;
        }
    }

    if(spravne == delka-1 && (strcmp(tabulka.rows[i].cells[j].data, "\0") != 0))
    {
        return 1;
    }
    return 0;

}

int isFloatTmp(tmp_t tmp)
{
    int spravne = 0;

    int delka = strlen(tmp.data);
    for(int q =0; q < delka;q++)
    {
        if((tmp.data[q] >= '0' && tmp.data[q] <= '9' ) || tmp.data[q] == '.' || tmp.data[q] == '-')
        {
            spravne++;
        }
        else
        {
           return -1;
        }
    }
    if(spravne == delka && (strcmp(tmp.data, "\0") != 0))
    {
        return 1;
    }
    return 0;

}
int countDigitCells(table_t tabulka, int i, int j, int * count)
{
    if(isFloat(tabulka, i, j) == 1)
    {
        *count = *count +1;
    }
    return *count;
}



int provedOperaci(selection_t selekce,table_t tabulka, commandAray_t polePrikazu , int i, int j, int q, float * sum, int * count, int * countDigits, tmpArray_t * poleTmp)
{
    if(strstr(polePrikazu.prikazy[q].data,"set") != NULL)
    {
       char STR[substringLenght(polePrikazu,q)];
       substring(polePrikazu, q, STR);


       if(tabulka.rows[i].cells[j].length < strlen(STR)+1)
       {
            tabulka.rows[i].cells[j].data = realloc(tabulka.rows[i].cells[j].data, (strlen(STR) * sizeof(char) + 1));
       }
       tabulka.rows[i].cells[j].length = strlen(STR)+1;
       strcpy(tabulka.rows[i].cells[j].data,STR);
    }

    if(strstr(polePrikazu.prikazy[q].data,"clear") != NULL)
    {
        free(tabulka.rows[i].cells[j].data);
        tabulka.rows[i].cells[j].data = malloc(sizeof(char) +1);
        strcpy(tabulka.rows[i].cells[j].data, "\0");
        tabulka.rows[i].cells[j].length = 1;
    }

    if(strstr(polePrikazu.prikazy[q].data,"swap") != NULL)
    {
        //swap chci provest pouze pokud je v selekci vybrana 1 bunka a ne oblast bunek
        if(isSelectedCell(selekce) != 1)
        {
            fprintf(stderr, "swap lze provest pouze pokud je selekce bunek nastavena na 1 bunku\n");
            return -1;
        }
        long poleSouradnic [2];
        getCordinates(polePrikazu.prikazy[q].data, poleSouradnic);


        //realokace nove
        if(tabulka.rows[i].cells[j].length < tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].length)
        {
            tabulka.rows[i].cells[j].data = realloc(tabulka.rows[i].cells[j].data, tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].length);
        }

        //realokace puvodni
        if(tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].length < tabulka.rows[i].cells[j].length)
        {
            tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data = realloc(tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data, tabulka.rows[i].cells[j].length);
        }

        int docasnaDelka = tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].length;
        char * docasnyText = tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data;

        tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].length = tabulka.rows[i].cells[j].length;
        tabulka.rows[poleSouradnic[0]-1].cells[poleSouradnic[1]-1].data = tabulka.rows[i].cells[j].data;

        tabulka.rows[i].cells[j].length = docasnaDelka;
        tabulka.rows[i].cells[j].data = docasnyText;

    }

     if(strstr(polePrikazu.prikazy[q].data,"sum") != NULL || strstr(polePrikazu.prikazy[q].data,"avg") != NULL )
     {
         *sum = *sum + atof(tabulka.rows[i].cells[j].data);
         *countDigits = countDigitCells(tabulka, i, j, countDigits);
     }
     if(strstr(polePrikazu.prikazy[q].data,"count") != NULL)
     {
         *count = countEmptyCells(tabulka,i,j, count);
     }
     if(strstr(polePrikazu.prikazy[q].data,"len") != NULL)
     {
        if(isSelectedCell(selekce) != 1)
        {
            fprintf(stderr, "len lze provest pouze pokud je selekce bunek nastavena na 1 bunku\n");
            return -1;
        }

        long poleSouradnic [2];
        getCordinates(polePrikazu.prikazy[q].data, poleSouradnic);


        int delka  = tabulka.rows[i].cells[j].length-1;
         replaceCellWithNumber(delka, tabulka, poleSouradnic);
     }

      if(strstr(polePrikazu.prikazy[q].data,"def") != NULL)
      {
          if(isSelectedCell(selekce) != 1)
          {
            fprintf(stderr,"prikaz def vyzaduje, aby byla vybrana pouze 1 bunka\n");
          }

          //-48 at nemusim azse prevadet na string aby sel pouzit atoi
          int pivot = (int)(polePrikazu.prikazy[q].data[polePrikazu.prikazy[q].length-1]) -48;

          poleTmp->tmps[pivot].data = tabulka.rows[i].cells[j].data;
      }
       if(strstr(polePrikazu.prikazy[q].data,"use") != NULL)
       {
           int pivot = (int)(polePrikazu.prikazy[q].data[polePrikazu.prikazy[q].length-1]) -48;

            //dosatne poleTmp -> realokuj velikost bunky do ktere chces nahrat -> naherj
            unsigned int novaDelka = strlen(poleTmp->tmps[pivot].data) +1;

            if(tabulka.rows[i].cells[j].length != novaDelka)
            {
                tabulka.rows[i].cells[j].data = realloc(tabulka.rows[i].cells[j].data, (novaDelka * sizeof(char)));
            }

            tabulka.rows[i].cells[j].length = novaDelka;
            strcpy(tabulka.rows[i].cells[j].data,poleTmp->tmps[pivot].data);
       }
        if(strstr(polePrikazu.prikazy[q].data,"inc") != NULL)
        {
             int pivot = (int)(polePrikazu.prikazy[q].data[polePrikazu.prikazy[q].length-1]) -48;

             if(isFloatTmp(poleTmp->tmps[pivot]) == 1)
             {
                float help = atof(poleTmp->tmps[pivot].data);

                help++;

                sprintf(poleTmp->tmps[pivot].data,"%g", help);

             }
             else
             {
                 strcpy(poleTmp->tmps[pivot].data, "1");

             }

        }
    return 0;
}

//v tehle funkci jsou prikazy vyzudujici nejdriv cele probehnuti selekce a pak az prepsani tabulky
int loadResultIntoTable (table_t tabulka, commandAray_t polePrikazu ,int q, float sum, int count, int countDigits)
{
    if(strstr(polePrikazu.prikazy[q].data,"sum") != NULL)
     {
         long poleSouradnic [2];
         getCordinates(polePrikazu.prikazy[q].data, poleSouradnic);

         if(countDigits >0)
         {
              replaceCellWithNumber(sum, tabulka, poleSouradnic);
         }
         else
         {
              fprintf(stderr, "ve vybrane selekci nejsou ciselne bunky\n");
              return -1;
         }
     }
     if(strstr(polePrikazu.prikazy[q].data,"avg") != NULL)
     {
         //nefunguje vypis do tabulky... asi malo mista nebo sprintf neumi. kazdopadne mam to vypisovat jako %g :o
         long poleSouradnic [2];
         getCordinates(polePrikazu.prikazy[q].data, poleSouradnic);

         float prumer = sum / countDigits;
         if(countDigits >0)
         {
              replaceCellWithNumber(prumer, tabulka, poleSouradnic);
         }
         else
         {
              fprintf(stderr, "ve vybrane selekci nejsou ciselne bunky\n");
              return -1;
         }
     }
     if(strstr(polePrikazu.prikazy[q].data,"count") != NULL)
     {
         long poleSouradnic [2];
         getCordinates(polePrikazu.prikazy[q].data, poleSouradnic);

         replaceCellWithNumber(count, tabulka, poleSouradnic);
     }
     return 0;
}

void addValuesToStructure(long * poleSouradnic, int pocetSouradnic, selection_t * selekce, table_t tabulka)
{
    int pocetCarek = pocetSouradnic-1;

    if(pocetCarek == 1)
    {
       if(poleSouradnic[0] == -1)
       {
           selekce->r1 = 1;
           selekce->r2 = tabulka.numRows;
       }
       else
       {
           selekce->r1 = poleSouradnic[0];
           selekce->r2 = selekce->r1;
       }

       if(poleSouradnic[1] == -1)
       {
           selekce->c1 = 1;
           selekce->c2 = tabulka.rows[0].numCells;
       }
       else
       {
           selekce->c1 = poleSouradnic[1];
           selekce->c2 = selekce->c1;
       }
    }
    if(pocetCarek == 3)
    {
        selekce->r1 = poleSouradnic[0];
        selekce->c1 = poleSouradnic[1];

        selekce->r2 = poleSouradnic[2];
        selekce->c2 = poleSouradnic[3];

       if(selekce->r1 == -1)
       {
            selekce->r1 = 1;
       }
        if(selekce->c1 == -1)
       {
            selekce->c1 = 1;
       }
       if(selekce->r2 == -1)
       {
            selekce->r2 = tabulka.numRows;
       }
        if(selekce->c2 == -1)
       {
            selekce->c2 = tabulka.rows[0].numCells;
       }
    }
}



int findIndexOfMax(selection_t selekce, table_t tabulka, long * poleSouradnic, int * pocetSouradnic)
{
    bool firstCorrect = false;
    float max;
    int newI;
    int newJ;

    for(int i = selekce.r1 -1; i <= selekce.r2-1; i++)
    {
         for(int j = selekce.c1 -1; j <= selekce.c2-1; j++)
         {
             if(firstCorrect == false)
             {
                 if(isFloat(tabulka,i,j) == 1)
                 {
                     max = atof(tabulka.rows[i].cells[j].data);
                     newI = i;
                     newJ = j;
                     firstCorrect = true;
                 }
             }
             else
             {
                if(isFloat(tabulka,i,j) == 1)
                {
                    if(atof(tabulka.rows[i].cells[j].data) > max)
                    {
                        newI = i;
                        newJ = j;
                        max = atof(tabulka.rows[i].cells[j].data);
                    }
                }

             }


         }
    }
    if(firstCorrect == true)
    {
        poleSouradnic[0] = newI+1;
        poleSouradnic[1] = newJ+1;
        *pocetSouradnic = 2;
        return 1;
    }
    return -1;
}

int findIndexOfMin (selection_t selekce, table_t tabulka, long * poleSouradnic, int * pocetSouradnic)
{
    bool firstCorrect = false;
    float min;
    int newI;
    int newJ;

    for(int i = selekce.r1 -1; i <= selekce.r2-1; i++)
    {
         for(int j = selekce.c1 -1; j <= selekce.c2-1; j++)
         {
             if(firstCorrect == false)
             {
                 if(isFloat(tabulka,i,j) == 1)
                 {
                     min = atof(tabulka.rows[i].cells[j].data);
                     newI = i;
                     newJ = j;
                     firstCorrect = true;
                 }
             }
             else
             {
                if(isFloat(tabulka,i,j) == 1)
                {
                    if(atof(tabulka.rows[i].cells[j].data) < min)
                    {
                        newI = i;
                        newJ = j;
                        min = atof(tabulka.rows[i].cells[j].data);
                    }
                }

             }


         }
    }
    if(firstCorrect == true)
    {
        poleSouradnic[0] = newI+1;
        poleSouradnic[1] = newJ+1;
        *pocetSouradnic = 2;
        return 1;
    }
    return -1;
}

int findStrInTable(table_t tabulka, commandAray_t polePrikazu, long * poleSouradnic, int * pocetSouradnic ,selection_t selekce, int q)
{
    char STR[substringLenght(polePrikazu,q)];
    substring(polePrikazu, q, STR);
    STR[strlen(STR)-1] = '\0';

    for(int i = selekce.r1 -1; i <= selekce.r2-1; i++)
    {
         for(int j = selekce.c1 -1; j <= selekce.c2-1; j++)
         {
             if(strstr(tabulka.rows[i].cells[j].data,STR))
             {
                poleSouradnic[0] = i+1;
                poleSouradnic[1] = j+1;
                *pocetSouradnic = 2;
                return 1;
             }
         }
    }
    fprintf(stderr,"v zadanem vyberu neni zadna bunka obsahujici '%s', takze se nemohl prikaz 'find %s' provest\n", STR, STR);
    return -1;
}

void deleteEmptyColumn(table_t tabulka)
{
    unsigned int pocetPrazdnych = 0;
    for(unsigned int i = 0; i < tabulka.numRows; i++)
    {
         for(unsigned int j = 0; j < tabulka.rows[0].numCells; j++)
         {
             if(strcmp(tabulka.rows[i].cells[j].data,"\0") == 0)
             {
                 pocetPrazdnych++;
                 break;
             }
         }
    }
    if(pocetPrazdnych == tabulka.numRows)
    {
        for(unsigned int i = 0; i < tabulka.numRows; i++)
        {
            free(tabulka.rows[i].cells[tabulka.rows[0].numCells-1].data);
        }
        tabulka.rows[0].numCells--;
    }
}

int main(int argc, char *argv[])
{
    int pozicePrikazu = 1;

    if(argc < 3)
    {
        fprintf(stderr, "program potrebuje argumenty, aby mohl spravne pracovat\n");
        return -1;
    }

    char *delim;
    if(strcmp(argv[1], "-d") == 0)
    {
        delim = argv[2];
        pozicePrikazu = pozicePrikazu +2;
    }
    else
    {
        delim = " ";
    }



    /* NACTENI PRIKAZU DO POLE */
    command_t prikaz;
    prikaz.length = 0;

    commandAray_t polePrikazu;
    polePrikazu.numCommands = 0;

    //udelat lepsi check, jestli jsou spravne argumenty
    char * input = argv[pozicePrikazu];


    int i =0;

    while(input[i] != '\0')
    {
        if(input[i] == ';' && (i >0 && input[i-1]!=92))//92 = \ (escape znak)
        {
          addCommandToArray(&polePrikazu, prikaz);
          addCharToCommand(&prikaz, '\0');
          prikaz.length = 0;
        }
        else
        {
            if(input[i] != 92)
            {
                 addCharToCommand(&prikaz, input[i]);
            }
        }
        i++;
    }
    addCommandToArray(&polePrikazu, prikaz);
    addCharToCommand(&prikaz, '\0');
    addCharToCommand(&prikaz, '\0');
    /*-- KONEC NACITANI PRIKAZU DO POLE --*/

    if(argc < (3+pozicePrikazu-1))
    {
        fprintf(stderr, "zadejte vsechny argumenty spravne\n");
        return -1;
    }
    FILE * soubor;
    soubor = fopen(argv[pozicePrikazu+1], "r");

    cell_t bunka;
    bunka.length = 0;

    row_t radek;
    radek.numCells = 0;

    table_t tabulka;
    tabulka.numRows = 0;



    /* ROZSEKANI VSTUPNIHO SOUBORU */
    char c;
    bool first = true;
    int celkovaDelka = 0;
    while((c = fgetc(soubor)) != EOF)
    {
        if(c == 10)//radsi tam dat \n
        {
            if(first == true)
            {
                celkovaDelka = radek.numCells;
                first = false;
            }

            //tenhle blok pridava posledni bunku do radku (protoze neni zakoncena DELIMEM
            addCharToCell(&bunka, '\0');
            addCellToRow(&radek, bunka);
            bunka.length = 0;


            //pridani bunek, pokud jich nektery radek ma min nez prvni
            int q = radek.numCells;
            while(q <= celkovaDelka)
            {
                addCharToCell(&bunka, '\0');//bacha ten radek ma alokovanou pamet jen na 1 znak
                addCellToRow(&radek, bunka);
                bunka.length = 0;
                q++;
            }

            addRowToTable(&tabulka, radek);
            radek.numCells = 0;

        }
        else if(isDelim(c, delim))
        {
            addCharToCell(&bunka, '\0');
            addCellToRow(&radek, bunka);
            bunka.length = 0;

        }
        else
        {
            if(c !=10 ||c != 13)
            {
                addCharToCell(&bunka, c);
            }
        }
    }
    /* -- KONEC ROZSEKAVANI VSTUPNIHO SOUBORU --*/


    /* PROVADENI PRIKAZU */
    selection_t selekce;
    inicializujSelekci(&selekce);

    float sum;
    int count;
    int countDigits;
    tmpArray_t poleTmp;
    for(unsigned int q = 0; q < polePrikazu.numCommands; q++)
    {
       sum = 0.0;
       count = 0;
       countDigits = 0;
       if(polePrikazu.prikazy[q].data[0] == '[' && polePrikazu.prikazy[q].data[polePrikazu.prikazy[q].length-1] == ']')
       {
            long poleSouradnic [4];
            int pocetSouradnic = 0;
            if(strstr(polePrikazu.prikazy[q].data,"find") == NULL)
            {
                pocetSouradnic = getCordinates(polePrikazu.prikazy[q].data, poleSouradnic);
            }


            if(pocetSouradnic == 0)
            {
                if(strstr(polePrikazu.prikazy[q].data,"max"))
                {
                    if(findIndexOfMax(selekce, tabulka, poleSouradnic, &pocetSouradnic) == -1)
                    {
                        fprintf(stderr,"v zadanem vyberu neni zadna ciselna bunka, takze se nemohl prikaz MAX provest\n");
                         return -1;
                    }
                }

                if(strstr(polePrikazu.prikazy[q].data,"min"))
                {
                    if(findIndexOfMin(selekce, tabulka, poleSouradnic, &pocetSouradnic) == -1)
                    {
                         fprintf(stderr,"v zadanem vyberu neni zadna ciselna bunka, takze se nemohl prikaz MIN provest\n");
                         return -1;
                    }
                }
                if(strstr(polePrikazu.prikazy[q].data,"find"))
                {
                    if(findStrInTable(tabulka, polePrikazu, poleSouradnic, &pocetSouradnic, selekce,q) == -1)
                    {
                        return -1;
                    }
                }
            }
            addValuesToStructure(poleSouradnic, pocetSouradnic, &selekce, tabulka);
       }
       else
       {
            for(int i = selekce.r1 -1; i <= selekce.r2-1; i++)
            {
                 for(int j = selekce.c1 -1; j <= selekce.c2-1; j++)
                 {
                      if(provedOperaci(selekce, tabulka, polePrikazu, i, j, q, &sum, &count, &countDigits, &poleTmp) == -1)
                      {
                          fprintf(stderr, "sekvence prikazu se zasekla na prikazu c. %d\n", q+1);

                          return -1;
                      }
                 }
            }
            loadResultIntoTable(tabulka, polePrikazu , q, sum, count, countDigits);
       }
    }


    //smazaze posledni prazdny sloupec
    deleteEmptyColumn(tabulka);

     fclose(soubor);//zavre soubor na cteni

     FILE * souborWrite;
    souborWrite = fopen(argv[pozicePrikazu+1], "w");//otevre soubor pro zapis

      vypisTabulku(tabulka, souborWrite, delim);


    /* UVOLNENI PAMETI */
    int pocetRadku = tabulka.numRows;
    int pocetSloupcu = tabulka.rows->numCells;
    for(int i =0; i < pocetRadku; i++)
    {
        for(int j = 0; j < pocetSloupcu; j++)
        {
             free(tabulka.rows[i].cells[j].data);
             tabulka.rows[i].cells[j].length = 0;
        }
        free(tabulka.rows[i].cells);
        tabulka.rows[i].numCells = 0;
    }
    free(tabulka.rows);
    tabulka.numRows = 0;

    int pocetPrikazu = polePrikazu.numCommands;
    for(int i =0; i < pocetPrikazu; i++)
    {
        free(polePrikazu.prikazy[i].data);
        polePrikazu.prikazy[i].length = 0;
    }
    free(polePrikazu.prikazy);
    polePrikazu.numCommands = 0;
    /* -- KONEC UVOLNOVANI PAMETI --*/

    fclose(soubor);


    return 0;
}
