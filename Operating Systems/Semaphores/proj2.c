#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/wait.h>
 

//deklarace semaforů
sem_t *semQueue;
sem_t *semElf;
sem_t *semSanta;
sem_t *semReindeer;
sem_t *semSantaSleep;
sem_t *semElfWork;
sem_t *semChristmas;


//deklarace globálních proměnných
int *commandNumber;
int *reindeerWaiting;
int *elfWaiting;
bool *closed;

FILE *file;


void checkArguments (char *argv[], int argc)
{
    if(argc != 5)
    {
        fprintf(stderr, "program requires 5 arguments to run (./proj2 NE NR TE TR)\n");
        exit(1);
    }
    if(atoi(argv[1]) >= 1000 || atoi(argv[1])  <= 0) //počet skřítků
    {
        fprintf(stderr, "number of elves must be between 1 and 999\n");
        exit(1);
    }
    if(atoi(argv[2]) >= 20 || atoi(argv[2])  <= 0) //počet sobů
    {
        fprintf(stderr, "number of deindeers must be between 1 and 19\n");
        exit(1);
    }
    if(atoi(argv[3]) > 1000 || atoi(argv[3])  < 0) //maximální pracovní doba skřítků
    {
        fprintf(stderr, "maximum worktime of elf must be between 0 and 1000\n");
        exit(1);
    }
    if(atoi(argv[4]) > 1000 || atoi(argv[4])  < 0) //maximální délka prázdnin sobů
    {
        fprintf(stderr, "maximum length of holidays of reindeer must be in interval 0 and 1000\n");
        exit(1);
    }
}


void printState(int i, char * object, char * msg)
{
    file = fopen("proj2.out", "a");

    if(strcmp("Santa", object) == 0)
    {
        sem_wait(semQueue);
        (*commandNumber)++;
        fprintf(file, "%d: %s: %s\n", *commandNumber, object, msg);
        sem_post(semQueue);
    }
    else
    {
        sem_wait(semQueue);
        (*commandNumber)++;
        fprintf(file, "%d: %s %d: %s\n", *commandNumber, object,i, msg);
        sem_post(semQueue);
    }
  

    fclose(file);
}

void doReindeerProcess(int reindeerTimeoutMax, int i, int numReindeer)
{
    printState(i, "RD", "rstarted");

    //pokud TR = 0, nemá cenu Reindeer proces uspávat na 0 ms
    if(reindeerTimeoutMax > 0)
    {
         srand(getpid());
        int timeout =  rand() % reindeerTimeoutMax;
        while(timeout < (reindeerTimeoutMax/2) || timeout > reindeerTimeoutMax)
        {
            timeout =  rand() % reindeerTimeoutMax;
        }
        timeout = timeout * 1000; //usleep očekává mikrosekundy, násobím 1000 abych neuspal proces na mikrosekundy, ale na milisekundy
        usleep(timeout);
    }
   

    printState(i, "RD", "return home");


    (*reindeerWaiting)++;
    if((*reindeerWaiting) == numReindeer)
    {
        sem_post(semSanta);
    }
    sem_wait(semReindeer);

    printState(i, "RD", "get hitched");

    (*reindeerWaiting)--;
    //pokud zapřáhnul posledního soba, začali vánoce
    if((*reindeerWaiting) == 0)
    {
        sem_post(semChristmas);
    }

    exit(0);   
}

void doElfProcess(int elfTimeoutMax, int i)
{
    bool start = true; //pomocná proměnná, aby se "started" neprintStateovalo více než 1x pro 1 proces

    //nekonečný cyklus, aby skřítci vyráběli hračky, dokud nezačnou vánoce
    while (true)
    {
        if(start == true)
        {
            printState(i, "Elf", "started");
        }

        //pokud TE = 0, nemá cenu Elf proces uspávat na 0 ms
        if(elfTimeoutMax > 0)
        {
            srand(getpid());
            int timeout =  rand() % elfTimeoutMax;

            timeout = timeout * 1000;//usleep očekává mikrosekundy, násobím 1000 abych neuspal proces na mikrosekundy, ale na milisekundy
            usleep(timeout);
        }
      
        printState(i, "Elf", "need help");  

        sem_wait(semElf);

        //pokud už santa zavřel workshop, tak i zbylé skřítky pošle na dovolenou (kdyby tento IF nebyl, na dovolenou by odešli jen 3 nejrychlejší)
        if((*closed) == true)
        {
            printState(i, "Elf", "taking holidays");  
            (*elfWaiting)--;
            sem_post(semElf);
            break;
        }

        (*elfWaiting)++;
        //když čekají 3 skřítci, vzbudí santu
        if((*elfWaiting) == 3)
        {
            sem_post(semSanta);
        }
        else
        {
            sem_post(semElf);
        }

        sem_wait(semElfWork);

        //pokud už santa zavřel workshop, tak 3 čekající pošle na dovolenou
        if((*closed) == true)
        {
            printState(i, "Elf", "taking holidays");  
            (*elfWaiting)--;
            sem_post(semElf);
            break;
        }
       

        printState(i, "Elf", "get help");  

        (*elfWaiting)--;
        //až když santa pomohl všem 3, může jít zase spát
        if((*elfWaiting) == 0)
        {
            sem_post(semSantaSleep);
            sem_post(semElf);
        }
        start = false;
       
    }
    exit(0);
}

void doSantaProcess(int numReindeer, int i)
{
    while(true)
    {
        printState(i, "Santa", "going to sleep");  

        sem_wait(semSanta);//santa čeká než je vzbuzen a pak zjišťuje, kdo ho vzbudil

        if((*reindeerWaiting) == numReindeer)
        {
            printState(i, "Santa", "closing workshop");  
            *closed = true;
            
            //zapřáhnutí všech sobů
            for(int j = 0; j < numReindeer; j++)
            { 
                sem_post(semReindeer);
            }

            //sdělení čekajícím skřítkům, že je zavřený workshop
            for(int j = 0; j < (*elfWaiting); j++)
            { 
                sem_post(semElfWork);
            }
            break;
        }

        if((*elfWaiting) == 3)
        {
            printState(i, "Santa", "helping elves");  
          
            for(int j = 0; j < 3; j++)
            {
                sem_post(semElfWork);
            }

            sem_wait(semSantaSleep);//čeká než všichni 3 dostanou pomoc
        }
    }
   
    sem_wait(semChristmas);
    printState(i, "Santa", "Christmas started");  
       
   
    exit(0);
}

void generateElves(int numElves, int elfTimeoutMax)
{
    for(int i = 1; i < numElves+1; i++)
    {
        pid_t elf = fork();
        if(elf == 0) //když je to potomek, vejde do vykonávací fce a už se nevrátí (nevygeneruje další proces)
        {
           doElfProcess(elfTimeoutMax,i);
        }
    }
}
void generateReindeers(int numReindeers, int reindeerTimeoutMax)
{
    for(int i = 1; i < numReindeers+1; i++)
    {
        pid_t reindeer = fork();
        if(reindeer == 0) //když je to potomek, vejde do vykonávací fce a už se nevrátí (nevygeneruje další proces)
        {
           doReindeerProcess(reindeerTimeoutMax, i, numReindeers);
        }
    }
}

void cleanAll()
{
    //vyčistění paměti
    sem_destroy(semQueue);
    sem_destroy(semReindeer);
    sem_destroy(semElf);
    sem_destroy(semSanta);
    sem_destroy(semSantaSleep);
    sem_destroy(semElfWork);
    sem_destroy(semChristmas);

    munmap(semQueue,sizeof(int*));
    munmap(semReindeer,sizeof(int*));
    munmap(semElf,sizeof(int*));
    munmap(semSanta,sizeof(int*));
    munmap(semSantaSleep,sizeof(int*));
    munmap(semElfWork,sizeof(int*));
    munmap(semChristmas,sizeof(int*));

    munmap(commandNumber,sizeof(int*));
    munmap(reindeerWaiting,sizeof(int*));
    munmap(elfWaiting,sizeof(int*));
    munmap(closed,sizeof(int*));
    munmap(file,sizeof(int*));
}
void initAll()
{
    file = fopen("proj2.out","w");

    //inicializace semaforů jako sdílené proměnné
    semQueue =mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(semQueue,1,1);

    semReindeer =mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(semReindeer,1,0);

    semSanta = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(semSanta,1,0);

    semElf = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(semElf,1,1);

    semSantaSleep = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(semSantaSleep,1,0);

    semElfWork = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(semElfWork,1,0);

    semChristmas = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(semChristmas,1,0);

    //inicializace sdílených proměnných
    commandNumber= mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    reindeerWaiting = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    elfWaiting =  mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    closed = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    file = mmap(NULL, sizeof(file), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

int main(int argc, char *argv[])
{
    checkArguments(argv, argc);

    int numElves = atoi(argv[1]);
    int numReindeers = atoi(argv[2]);
    int elfTimeoutMax = atoi(argv[3]);
    int reindeerTimeoutMax = atoi(argv[4]);

    initAll();
   
    pid_t santa = fork();
    if(santa == 0)
    {
        doSantaProcess(numReindeers, 0);
    }
    
    generateElves(numElves,elfTimeoutMax);
    generateReindeers(numReindeers,reindeerTimeoutMax);

    //while cyklus, aby main proces skončil poslední
    while(wait(NULL) != -1 || errno != ECHILD);

    //vyčištění paměti
    cleanAll();

    exit(0);
    return 0;
}