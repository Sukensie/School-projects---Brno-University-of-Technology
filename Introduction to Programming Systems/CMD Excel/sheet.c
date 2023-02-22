#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define LINE_LENGTH 10242

/* IZP - projekt 1  TOMÁŠ SOUČEK xsouce15 */

//funkce kontrolující, zda je zadané písmeno jeden z oddělovačů
int isDelim(char pismeno, char * delim)
{
    int wasDelim = 0;
    int delimLength =  strlen(delim);

    //for cyklus, protože nevím jestli bude DELIM jeden znak, nebo více
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

//funkce rozsekává řádek tabulky na buňky
char *myStrtok(char* buffer, char * delim, int * delkaPole)
{
    if(buffer[0] == '\0' ||buffer[0] == '\n')
    {
        return NULL;
    }
    for(int i = 0; buffer[i]+1; i++)
    {
        if(isDelim(buffer[i], delim) == 1)
        {
            buffer[i] = '\0';
             *delkaPole = *delkaPole +1; //zjišťování počtu buněk
        }
        if(buffer[i] == 13)
        {
            buffer[i] = '\0';
        }
        if(buffer[i] == '\0')
        {
            return buffer;//vrátí obsah buňky
        }
    }
    return NULL;
}

//funkce ukládá buňky do pole
char **nactiPole (char * buffer, char * delim, char * pole[], int * delkaPole)
{
    *delkaPole = 1;
    char * foo = myStrtok(buffer,delim,delkaPole);
    pole[0] = foo;
    int pivot = 1;
    while(foo != NULL)
    {
        foo = myStrtok(foo + strlen(foo) + 1, delim, delkaPole);
        pole[pivot] = foo;
        pivot++;
    }
    return pole;
}

//funkce pro kontrolu, zda buňka obsahuje číslo nebo číslo s desetinným místem
int isFloat(char *poleSlov[], int number)
{
    //za float považuji vstup který obsahuje buď číslice nebo tečku
    int spravne = 0;
    int delka = strlen(poleSlov[number]);
    for(int i =0; i < delka;i++)
    {
        if((poleSlov[number][i] >= '0' && poleSlov[number][i] <= '9')|| poleSlov[number][i] == '.')
        {

            spravne++;
        }
        else
        {
           fprintf(stderr, "bunka neobsahuje cislo\n");
           break;
        }
    }
    if(spravne == delka)
    {
        return 1;
    }
    else
    {
         return 0;
    }
}

//zaokroulovací funkce
int myRound(float x)
{
    if (x >= 0)
    {
        return (int) (x + 0.5);
    }
    return (int) (x - 0.5);
}

//funkce na vypsání pole(jednoho řádku)
void vypisPole (char * pole[], char * delim, int edit, int cislo, int cislo2)
{
    //edit=1...icol || edit=2...acol || edit=3...dcol || edit=4...dcols
    //hodnotu edit dostává z funkce upravaTabulky

     bool povoleni = true;//pokud je zadaná funkce na např. smazání sloupce, tak se povoleni pro vypsání konkrétní buňky nastaví na false
     bool printDelim = false;
     for(int j=0; pole[j] != NULL; j++)
     {
         povoleni = true;
         if(edit == 1)
         {
             if(j == cislo)
             {
                  printf("%c", delim[0]);
             }
         }

         if(edit == 3)
         {
             if(j == cislo)
             {
                 povoleni= false;
             }
         }

         if(edit == 4)
         {
             if(j >= cislo &&j <= cislo2)
             {
               povoleni =false;
             }
         }


         if(povoleni == true)
         {
            //delim chci vytisknout až před druhou buňku
            if(printDelim == true)
            {
                printf("%c", delim[0]);
            }
             printf("%s", pole[j]);
            printDelim=true;
         }
     }
     //acol má přidat sloupec vždy až na konec řádku, proto je až po ukončení vypisovacího cyklu
     if(edit == 2)
     {
         printf("%c", delim[0]);
     }
}

//funkce která provádí jednotlivé operace ze sekce Zpracování dat
int provedOperaci(char * pole[], char * argv[], int pozice, int argc, int delkaPole)
{
    int cislo;

    if(argc > pozice+1)
    {
        if(atoi(argv[pozice+1]) > 0)
        {
            cislo = atoi(argv[pozice+1]);
            cislo--;
            if(cislo >= delkaPole)
            {
                 fprintf(stderr, "tento radek tabulky nema pocet sloupcu %d\n", cislo+1);
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "zadane cislo musi byt vetsi nez 0\n");
            return -1;
        }

    }
    else
    {
         fprintf(stderr, "malo argumentu\n");
         return -1;
    }



    if(strcmp(argv[pozice], "cset") == 0)
    {
        //tady dávám kontorlu dovnitř, protože CSET potřebuje k sobě další 2 argumenty, narozdíl od většiny ostatních
        if(argc > pozice+2)
        {
            pole[cislo] = argv[pozice+2];
        }
        else
        {
            fprintf(stderr, "malo argumentu\n");
            return -1;
        }

    }
    else if(strcmp(argv[pozice], "tolower") == 0)
    {
        int delkaSTR =  strlen(pole[cislo]);
        for(int j = 0; j < delkaSTR;j++)
        {
              pole[cislo][j] = tolower(pole[cislo][j]);
        }

    }
    else if(strcmp(argv[pozice], "toupper") == 0)
    {
        int delkaSTR =  strlen(pole[cislo]);

        for(int j = 0; j < delkaSTR;j++)
        {
             pole[cislo][j] = toupper(pole[cislo][j]);

        }
    }
    else if(strcmp(argv[pozice], "round") == 0)
    {
        //zkotroluje jestli je zadana bunka číslo
        if(isFloat(pole, cislo) == 1)
        {
            //konvertuj string na float, abys mohl dělat matematické operace
            float a = atof(pole[cislo]);
            int rounded = myRound(a);
            //int musí převést zpět na string, aby se dal uložit do pole stringù
            sprintf(pole[cislo],"%d", rounded);
        }
        else
        {
            fprintf(stderr, "bunka neobsahuje cislo");
            return -1;
        }

    }
    else if(strcmp(argv[pozice], "int") == 0)
    {
         if(isFloat(pole, cislo) == 1)
         {
             int a = atoi(pole[cislo]);
            sprintf(pole[cislo],"%d", a);
         }
        else
        {
            fprintf(stderr, "bunka neobsahuje cislo");
            return -1;
        }

    }
    else if(strcmp(argv[pozice], "swap") == 0)
    {
        if(argc > pozice+2)
        {
            if(atoi(argv[pozice+2]) > 0)
            {
                char * help;
                char * N = argv[pozice+1];
                char * M = argv[pozice+2];

                int n = atoi(N);
                n--; // protože indexuji od 1, ne od 0

                int m = atoi(M);
                m--;

                //kontroluji pouze M, protože N už je zkontrolované na začátku funkce
                if(m >= delkaPole)
                {
                     fprintf(stderr, "tento radek tabulky nema pocet sloupcu %d\n", m+1);
                    return -1;
                }

                help = pole[n];
                pole[n] = pole[m];
                pole[m] = help;
            }
            else
            {
                fprintf(stderr, "zadane cislo musi byt vetsi nez 0\n");
                return -1;
            }

        }
        else
        {
            fprintf(stderr, "swap malo argumentu\n");
            return -1;
        }

    }
    else if(strcmp(argv[pozice], "copy") == 0)
    {
        if(argc > pozice+2)
        {
            if(atoi(argv[pozice+2]) > 0)
            {
                char * N = argv[pozice+1];
                char * M = argv[pozice+2];

                int n = atoi(N);
                n--;

                int m = atoi(M);
                m--;

                if(m >= delkaPole)
                {
                     fprintf(stderr, "tento radek tabulky nema pocet sloupcu %d\n", m+1);
                    return -1;
                }

                pole[m] = pole[n];
            }
            else
            {
                fprintf(stderr, "zadane cislo musi byt vetsi nez 0\n");
                return -1;
            }

        }
        else
        {
            fprintf(stderr, "malo argumentu\n");
            return -1;
        }
    }
    else if(strcmp(argv[pozice], "move") == 0)
    {
        if(argc > pozice+2)
        {
            if(atoi(argv[pozice+2]) > 0)
            {
                char * N = argv[pozice+1];
                char * M = argv[pozice+2];

                int n = atoi(N);
                n--;

                int m = atoi(M);
                m--;

                if(m >= delkaPole)
                {
                     fprintf(stderr, "tento radek tabulky nema pocet sloupcu %d\n", m+1);
                     return -1;
                }


                //pokud N a M jsou stejné, tak nemá smysl nic přesouvat
                if(n != m)
                {
                    char * help;
                    help = pole[n];

                    if(n >m)
                    {
                         for(int j= n; j > m; j--)
                         {
                            help = pole[j-1];
                            pole[j-1] = pole[j];
                            pole[j] = help;
                         }
                    }
                    else
                    {
                        for(int j= n; j < m-1; j++)
                        {
                            help = pole[j+1];
                            pole[j+1] = pole[j];
                            pole[j] = help;
                        }
                    }
                }
            }
            else
            {
                fprintf(stderr, "zadane cislo musi byt vetsi nez 0\n");
                return -1;
            }

        }
        else
        {
            fprintf(stderr, "malo argumentu\n");
            return -1;
        }
    }
    return 1;
}

//funkce která provádí jednotlivé operace ze sekce Selekce řádků
int provedSelekci (char * pole[], char * argv[], int argc, int pozicePrikazu, int i, int delkaPole)
{
    if(argc <= pozicePrikazu+1)
    {
        fprintf(stderr, "prikaz na selekci ma malo argumentu\n");
        return -1;
    }

    if(strcmp(argv[pozicePrikazu-3], "rows") == 0)
        {
            if(atoi(argv[pozicePrikazu-2]) > 0 && (atoi(argv[pozicePrikazu-1]) > 0 ||  strcmp(argv[pozicePrikazu-1], "-") == 0))
            {
                char * N = argv[pozicePrikazu-2];
                char * M = argv[pozicePrikazu-1];

                int m = atoi(M);
                if(strcmp(M, "-") == 0)
                {
                    m = i;
                }

                int n = atoi(N);
                if(i >= n && i <= m)
                {
                    provedOperaci(pole,argv, pozicePrikazu,argc, delkaPole);
                }
                return 1;

            }
            else
            {
                fprintf(stderr, "prikaz rows potrebuje 2 cisla\n");
                return -1;
            }

        }

        if(strcmp(argv[pozicePrikazu-3], "beginswith") == 0)
        {
            char * C = argv[pozicePrikazu-2];
            char * STR  = argv[pozicePrikazu-1];

            int c = atoi(C);
            c--;
            int delkaSTR = strlen(STR);
            int obsahuje = 0;

            for(int i=0; i < delkaSTR; i++)
            {
                if(STR[i] == pole[c][i])
                {
                    obsahuje++;
                }
            }
            if(obsahuje == delkaSTR)
            {
               provedOperaci(pole,argv, pozicePrikazu, argc, delkaPole);
            }
            return 1;
        }

        if(strcmp(argv[pozicePrikazu-3], "contains") == 0)
        {
            char * C = argv[pozicePrikazu-2];
            char * STR  = argv[pozicePrikazu-1];

            int c = atoi(C);
            c--;
            if(strstr(pole[c], STR) != NULL)
            {
                provedOperaci(pole,argv, pozicePrikazu, argc, delkaPole);
            }
              return 1;
        }
        return 0;
}

//funkce která provádí jednotlivé operace ze sekce Úprava tabulky
int upravaTabulky(char * argv[], char * delim, int pozicePrikazu, char * pole[], int i, bool * vypis, int  * cislo, int * cislo2, int  * edit, int argc, bool * wasArow)
{
    int obsahuje = 0;
    for(int f = pozicePrikazu; f < argc; f++)
    {
      if(strcmp(argv[pozicePrikazu], "irow") == 0)
        {
            char * R = argv[pozicePrikazu+1];
            int r = atoi(R);
            if(r>0 && r == i)
            {
                for(int j=0; pole[j] != NULL; j++)
                {
                    printf("%c", delim[0]);
                }
                printf("\n");
            }

            pozicePrikazu += 2;//konstrukce příkazu IROW se vždy musí skládat z 2 prvků, proto pozici příkazu zvětšuju o 2... inkrementace je z důvodu možnosti zadání sekvence příkazů
            f+=2;

            obsahuje++;
            //kontrola po inkrementaci, zda nebudu na dalším příkazu neooprávněně přistupovat do paměti
            if(pozicePrikazu > argc-1)
            {
               break;
            }
        }

        if(strcmp(argv[pozicePrikazu], "drow") == 0)
        {
             char * R = argv[pozicePrikazu+1];
             int r = atoi(R);

             if(i == r)
             {
                 *vypis = false;
             }
             pozicePrikazu += 2;
             f+=2;
             obsahuje++;
             if(pozicePrikazu > argc-1)
             {
                 break;
             }
        }


        if(strcmp(argv[pozicePrikazu], "drows") == 0)
        {
            char * N = argv[pozicePrikazu+1];
            char * M = argv[pozicePrikazu+2];

            int n = atoi(N);
            int m = atoi(M);

            if(m < n)
            {
                 fprintf(stderr, "M musi byt vetsi nez N");
            }

            if(i >= n && i <= m)
            {
                *vypis = false;
            }

            pozicePrikazu += 3;
            f+=3;
            obsahuje++;
            if(pozicePrikazu > argc-1)
            {
                break;
            }
        }

        if(strcmp(argv[pozicePrikazu], "dcol") == 0)
        {
            *edit = 3;
            *cislo = atoi(argv[pozicePrikazu+1]);

             pozicePrikazu += 2;
             f+=2;
             obsahuje++;
             if(pozicePrikazu > argc-1)
             {
                 break;
             }
        }


        if(strcmp(argv[pozicePrikazu], "icol") == 0)
        {
           *edit = 1;
           *cislo = atoi(argv[pozicePrikazu+1]);

            pozicePrikazu += 2;
            f+=2;
            obsahuje++;
            if(pozicePrikazu > argc-1)
            {
                break;
            }
        }

        if(strcmp(argv[pozicePrikazu], "acol") == 0)
        {
             *edit = 2;

             pozicePrikazu += 1;
             f+=1;
             obsahuje++;
             if(pozicePrikazu > argc-1)
             {
                 break;
             }
        }

        if(strcmp(argv[pozicePrikazu], "dcols") == 0)
        {
            *edit = 4;
            *cislo = atoi(argv[pozicePrikazu+1]);
            *cislo2 = atoi(argv[pozicePrikazu+2]);

             pozicePrikazu += 3;
             f+=3;
             obsahuje++;
             if(pozicePrikazu > argc-1)
             {
                 break;
             }
        }
        if(strcmp(argv[pozicePrikazu], "arow") == 0)
        {
            *wasArow = true;

             pozicePrikazu += 1;
              f+=1;
             obsahuje++;
             if(pozicePrikazu > argc-1)
             {
                 break;
             }
        }
    }

    //pokud v argumentech byl alespoň 1 příkaz na úpravu tabulky
    if(obsahuje > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int main(int argc, char *argv[])
{
    int pozicePrikazu = 1; //proměnná do které ukládám pozici aktuálně zpracovávaného příkazu (argumentu)...začínám od 1, protože argument 0 je cesta k souboru

    if(argc < 2)
    {
        fprintf(stderr, "program potrebuje argumenty, aby mohl spravne pracovat");
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
        delim = " ";//pokud není zadaný delim, pak se jako delim bere mezera
    }

    if(strcmp(argv[pozicePrikazu], "rows") == 0 || strcmp(argv[pozicePrikazu], "beginswith") == 0 || strcmp(argv[pozicePrikazu], "contains") == 0)
    {
        pozicePrikazu = pozicePrikazu + 3; //+3 protože každý pøíkaz na selekci řádků obsahuje 3 argumenty
    }

    char buffer[LINE_LENGTH];
    char * pole [1000];

    bool wasArow = false;
    bool vypis; //některé řádky nebo sloupce vypsat nechci (tím je jakoby smažu)
    int edit = -1; //viz funkce vypisPole
    int cislo = -1;
    int cislo2 = -1;
    int rozcesti= -1; //switch mezi příkazy na úpravu tabulky, nebo zpracováním dat... 1 = uprava tabulky, -1 = zpracování dat
    int delkaPole = 1;



    for (int i = 1; fgets(buffer, LINE_LENGTH, stdin); i++)
    {
        vypis = true;
        nactiPole(buffer, delim, pole, &delkaPole);//načti konkrétní řádek


        //do funkce posílám některé proměnné jako odkazy, protože je ve funkci měním a po funkci chci pracovat se změněnýma hodnotama
        if(upravaTabulky(argv, delim, pozicePrikazu, pole,i, &vypis, &cislo, &cislo2, &edit, argc, &wasArow) == 1)
        {
            rozcesti = 1;
        }

        if(rozcesti != 1)
        {
            if(provedSelekci(pole,argv,argc, pozicePrikazu,i, delkaPole) < 0)
            {
                 return -1;
            }
            if(provedSelekci(pole,argv,argc, pozicePrikazu,i, delkaPole) == 0)
            {
                if(provedOperaci(pole,argv, pozicePrikazu,argc, delkaPole) == -1)
                {
                    return -1;
                }
            }
        }


       if(vypis == true)
       {
             vypisPole(pole, delim, edit, cislo, cislo2);
             printf("\n");
       }

    }

    //arow je až po skončení načítání a vypisování celé tabulky, protože přidává řádek vždy až na konec tabulky
    if(wasArow == true)
    {
        for(int j=0; pole[j] != NULL; j++)
        {
            printf("%c", delim[0]);
        }
         printf("\n");
    }

    return 0;
}
