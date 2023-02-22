#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../library.h"

//https://stackoverflow.com/questions/15444567/removing-spaces-and-special-characters-from-string
static void convertToBase64(char *data)
{
    unsigned long i = 0; /* Scanning index */
    unsigned long x = 0; /* Write back index */
    char c;

    /*
     * Store every next character in `c` and make sure it is not '\0'
     * because '\0' indicates the end of string, and we don't want
     * to read past the end not to trigger undefined behavior.
     * Then increment "scanning" index so that next time we read the
     * next character.
     */ 
    while ((c = data[i++]) != '\0') {
        /* Check if character is either alphabetic or numeric. */
        if (isalnum(c) || c == '+' || c == '/' || c == '='  || c == '.') {
            /*
             * OK, this is what we need. Write it back.
             * Note that `x` will always be either the same as `i`
             * or less. After writing, increment `x` so that next
             * time we do not overwrite the previous result.
             */
            data[x++] = c;
        }
        /* else — this is something we don't need — so we don't increment the
           `x` while `i` is incremented. */
    }
    /* After all is done, ensure we terminate the string with '\0'. */
    data[x] = '\0';
}

    

int main(int argc, char *argv[]) 
{
    //dns_receiver {BASE_HOST} {DST_FILEPATH}

    //parsování argumentů
    if(argc != 3)
    {
        fprintf(stderr, "Prosím zadejte argumenty ve tvaru {BASE_HOST} {DST_FILEPATH}\n");
        return -1;
    }

    char* BASE_HOST = argv[1];
    char* DST_FILEPATH = argv[2];

    //https://www.geeksforgeeks.org/udp-server-client-implementation-c/
    int sockfd; 
    unsigned char* qname;
    char buffer[65536];
    struct DNS_HEADER *dns = NULL;
    struct QUESTION *qinfo = NULL;

    struct sockaddr_in servaddr, cliaddr; 
        
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
        
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(53); 
        
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    int receivedCount = 0;

    //vytvoření složky
    //https://stackoverflow.com/a/7430262
    struct stat st = {0};
    if (stat(DST_FILEPATH, &st) == -1) //kontrola, jestli už složka neexistuje, pokud neexistuje tak vytvoří
    {
        if(mkdir(DST_FILEPATH, 0777) == -1)
        {
            printf("nepodařilo se vytvořit složku\n");
            return -1;
        }
    }

    char* filePath;
    int totalFileSize = 0;
    int keepGoing = 1;

    while(keepGoing)
    {
        unsigned int len, n; 
    
        len = sizeof(cliaddr);  //len is value/result 
        n = recvfrom(sockfd, (char *)buffer, 65536, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);         

        dns = (struct DNS_HEADER *)&buffer;
        qname =(unsigned char*)&buffer[sizeof(struct DNS_HEADER)];
        qinfo =(struct QUESTION*)&buffer[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; //fill it

        buffer[n] = '\0'; 
    
        convertToBase64((char*)qname);

        char* data2;
        data2 = malloc(100*sizeof(char));
        if (!data2)
        {
            printf("Malloc failed\n");
            return -1;
        }

        //kontrola stejnosti BASE_HOST zaslaných ze senderu a argumentů pro receiver
        if(checkBaseHosts(BASE_HOST, qname) != 1)
        {
            return -1;
        }

        strncpy(data2, (char*)qname, strlen((char*)qname) - strlen(BASE_HOST) + 1);
        data2[strlen((char*)qname) - strlen(BASE_HOST) + 1 ] = '\0';
 
        char       *out;
        size_t      out_len;

        // +1 for the NULL terminator.  
        out_len = b64_decoded_size(data2)+1;

        out = malloc(out_len + 1);
        if(!out) 
        {
            printf("problem allocating out\n");
        }

        int code = b64_decode(data2, (unsigned char *)out, out_len);
    

        if (code != 1) {
            printf("fail code: %d\n", code);
            printf("Decode Failure\n");
            return 1; 
        }
        out[out_len] = '\0';

        totalFileSize += strlen((const char*)data2);

        if(receivedCount == 0)
        {
            dns_receiver__on_transfer_init(&cliaddr.sin_addr);

            filePath = malloc((strlen(out) + strlen(DST_FILEPATH) + 2) * sizeof(char));
            if(!filePath)
            {
                printf("filepath malloc failed\n");
                return -1;
            }

            strcat(filePath, DST_FILEPATH);
            strcat(filePath, "/");
            strcat(filePath, out);
            dns_receiver__on_chunk_received(&cliaddr.sin_addr, filePath, receivedCount, strlen(data2));
        
            dns_receiver__on_query_parsed(filePath, out);

            fclose(fopen(filePath, "w")); //vymazat data souboru, aby pro každé zavolání programu neprobíhalo appendování do bývalého souboru

            receivedCount++;

            if( sendto(sockfd, data2, strlen(data2), MSG_CONF, (struct sockaddr *) &cliaddr, len) < 0)
            {
                perror("sendto failed");
            } 


            //uvolnení paměti
            free(out);
            free(data2);
            out = NULL;
            data2 = NULL;
            continue;
        }

        dns_receiver__on_chunk_received(&cliaddr.sin_addr, filePath, receivedCount, strlen(data2));
        dns_receiver__on_query_parsed(filePath, data2);

        if(strncmp("|EXIT|", out,6) == 0)
        {
            keepGoing = 0;
        
            if( sendto(sockfd, data2, strlen(data2), MSG_CONF, (struct sockaddr *) &cliaddr, len) < 0)
            {
                perror("sendto failed");
            } 

            //uvolnení paměti
            free(out);
            free(data2);
            out = NULL;
            data2 = NULL;

            break;
        }
        else
        {
            FILE* f = fopen(filePath, "a");  
            if (f != NULL)                      
            {
                fprintf(f, "%s", out);            
                fclose(f);                      
                f = NULL;                      
            }
        }
        
        if( sendto(sockfd, data2, strlen(data2), MSG_CONF, (struct sockaddr *) &cliaddr, len) < 0)
        {
            perror("sendto failed");
        } 

        //uvolnení paměti
        free(out);
        free(data2);
        out = NULL;
        data2 = NULL;

        receivedCount++;
    }
    dns_receiver__on_transfer_completed(filePath, totalFileSize);

    //https://stackoverflow.com/questions/17013067/c-delete-last-n-characters-from-file
    //odstranění přebytečných charů na konci souboru
    int charsToDelete = 1;
    FILE* InputFile = fopen(filePath, "a");  
    fseek(InputFile,-charsToDelete,SEEK_END);
    off_t  position = ftell(InputFile);
    ftruncate(fileno(InputFile), position);
    fclose(InputFile);


    free(filePath);
    
    return 0; 

}