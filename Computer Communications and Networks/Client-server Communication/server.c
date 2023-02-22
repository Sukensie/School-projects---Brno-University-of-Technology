#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

char message [1024]; //globální proměnná pro obsah hlavičky

void hostname()
{
    FILE *fileStream; 
    char fileText [100];

    fileStream = fopen("/proc/sys/kernel/hostname", "r");
    fgets(fileText, 100, fileStream);
    fclose(fileStream); 

    //nahrání obsahu hlavičky
    strcpy(message,"HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n");
    strcat(message, fileText);
}

void cpuname ()
{
    FILE *fileStream; 
    char fileText [100];

    fileStream =  popen("cat /proc/cpuinfo | grep \"model name\" | head -n 1 | awk '{split($0, array, \": \"); print array[2]}'", "r"); //otevře soubor s info o cpu, vybere řádek s názvem, oddělí popis od názvu a název uloží do proměnné
    fgets(fileText, 100, fileStream);
    fclose(fileStream); 

    //nahrání obsahu hlavičky
    strcpy(message,"HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n");
    strcat(message, fileText);   
}


void cpuload()
{
    FILE *fileStream; 
    char fileText [100];
    char* token;
    long int idle = 0, sum = 0, prevIdle = 0, prevSum = 0;
    float nonIdle;

    for(int q  = 0; q < 2; q++, sum = 0)
    {
        //otevření souboru se statistikami procesu, přečtení prnvího řádku (suma všech vláken) a zavření
        fileStream = fopen("/proc/stat", "r");
        fgets (fileText, 100, fileStream);
        fclose(fileStream);


        token = strtok(fileText," ");//useknutí cpu sloupce

        //rozsekání řádku podle kategorii (viz https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux) a jejich sečtení
        for(int i = 0; token!=NULL; i++)
        {
            token = strtok(NULL," ");
            if(token!=NULL)
            {
                sum += atoi(token);

                if(i==3)
                {
                    idle = atoi(token);//idle stav si chci zachovat separátně pro následný výpočet využítí cpu
                }
            }
        }
        
        //pokud se jedná o druhou iteraci, nechci provádět ostaření hodnot ani prodlevu mezi čteními
        if(q != 0)
        {
            break;
        }
        
        //ostaření hodnot
        prevIdle = idle;
        prevSum = sum;
       
        sleep(1);//sekundová prodleva mezi jednotlivými čteními
    }
    
    nonIdle = (1-(((float)idle - (float)prevIdle) / ((float)sum - (float)prevSum)))*100; //přetypování je kritické, bez něj to špatně počítá

    
    //nahrání obsahu hlavičky
    strcpy(message,"HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n");
    char string [100];
    sprintf(string, "%0.2f %%\n", nonIdle);
    strcat(message, string); 
}


int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "server cant run wihtout parameters! Please add port number\n");
        return -1;
    }

    int port = atoi(argv[1]);
    if(port < 0 || port > 65535)
    {
        fprintf(stderr, "Please use port in range [0-65535]\n");
        return -1;
    }

    //inicializace server socketu
    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (const struct sockaddr *) &serverAddr, sizeof(serverAddr));

    listen(serverSocket, 420);

    while(1)
    {
        //inicializace klientského socketu při každém requestu
        int clientSocket;
        clientSocket = accept(serverSocket, NULL, NULL);
        recv(clientSocket, message, 1024, 0);
         
        if(strncmp(message,"GET /hostname ",14) == 0)
        {
            hostname();
        }
        else if(strncmp(message,"GET /cpu-name ",14) == 0)
        {
            cpuname();
        }
        else if(strncmp(message,"GET /load ",10) == 0)
        {
            cpuload();      
        }
        else
        {
            strcpy(message,"HTTP/1.1 400 Bad request\r\nContent-Type: text/plain;\r\n\r\nBad request\n");
        }
        
        send(clientSocket, message, strlen(message), 0 );
        close(clientSocket);
    }


    close(serverSocket);
    return 0;
}