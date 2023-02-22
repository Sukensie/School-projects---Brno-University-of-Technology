#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/time.h>

#include "../library.h"
#define BUF_SIZE 1024


int sendPacket(char* packetString, char* BASE_HOST, char* UPSTREAM_DNS_IP, int* totalFileSize, int packetId);




int main(int argc, char *argv[]) 
{
    //dns_sender [-u UPSTREAM_DNS_IP] {BASE_HOST} {DST_FILEPATH} [src_data]

    //parsování argumentů
    if(argc > 6 || argc < 3)
    {
        fprintf(stderr, "Prosím zadejte argumenty ve tvaru [-u UPSTREAM_DNS_IP] {BASE_HOST} {DST_FILEPATH} [src_data]\n");
        return -1;
    }
    int offset = 0;
    char* UPSTREAM_DNS_IP = "";

    //určení adresy příjemce
    if(argc >= 5)
    {
        offset = 2;
        UPSTREAM_DNS_IP = argv[2];
    }
    else
    {
        //get dns server
        //https://www.binarytides.com/dns-query-code-in-c-with-linux-sockets/?fbclid=IwAR0PYjXl9dIjuo3er2CgmPh31zJ2cl6U7twNHAfO3_r3VLzj8EZg5_H7ah4
        FILE *fp;
        char line[200];
        if((fp = fopen("/etc/resolv.conf" , "r")) == NULL)
        {
            printf("Failed opening /etc/resolv.conf file \n");
        }
        
        while(fgets(line , 200 , fp))
        {
            //přeskoč řádky kde jsou komentáře
            if(line[0] == '#')
            {
                continue;
            }
            if(strncmp(line , "nameserver" , 10) == 0)
            {
                UPSTREAM_DNS_IP = strtok(line , " ");
                UPSTREAM_DNS_IP = strtok(NULL , " ");
                break;
            }
        }
        fclose(fp);
    }
    
    char* BASE_HOST = argv[1+offset];
    char* DST_FILEPATH = argv[2+offset];
    char *src_data = malloc(sizeof(char) * BUF_SIZE);
    FILE* source;


    //určení zdroje pro čtení dat
    if(argc == 5 || argc == 3)
    {
        source = stdin;
    }
    else
    {
        FILE *file;
        if((file= fopen(argv[3+offset],"r")) == NULL)
        {
            printf("Failed opening source file \n");
            return -1;
        }
        source = file; 
    }





    int count = 0; //počitadlo přečtených znaků
    char c; //znak do kterého se bude načítat skrze getc
    char* packetString;
    packetString = (char*)malloc(70*sizeof(char)); //idk proč 70... TODO (bylo myslím nějakých 63 + \0 + buffer kvůli segfaultu)
            
    if (!packetString)
    {
        printf("Malloc failed\n");
        return -1;
    }


    int totalFileSize = 0; //celková velikost odeslaných dat v B
    int sentCount = 1; //počet odeslaných packetů - používám také jako ID packetů pro debug. Začínám od 1, protože napřed pošlu packet se jménem souboru

    on_transfer_init(UPSTREAM_DNS_IP);

    sendPacket(DST_FILEPATH, BASE_HOST, UPSTREAM_DNS_IP, &totalFileSize, 0);//packet se jménem souboru

    while ((c = getc(source)) != EOF)
    {
        strncat(packetString, &c, 1);
         
        //je tam 4:3 encoding ratio

        char next = getc(source); //kontrola jestli s dalším znakem už skončí soubor, abych případně mohl do paketu vložit zbytek dat
        if(count == 43 || next == EOF) //44 protože větší pakety už jsou malformed
        {
			strncat(packetString, &next, 1);

            sendPacket(packetString, BASE_HOST, UPSTREAM_DNS_IP, &totalFileSize, sentCount);
            

            memset(packetString,0,strlen(packetString));
            sentCount++;

            count = 0;
            continue;
        }
        count++;
        ungetc(next, source);
    }
    sendPacket("|EXIT|", BASE_HOST, UPSTREAM_DNS_IP, &totalFileSize, sentCount);//packet pro ukončení přenosu

    dns_sender__on_transfer_completed(DST_FILEPATH, totalFileSize);
    free(src_data);
    free(packetString);

}

int sendPacket(char* packetString, char* BASE_HOST, char* dstAddress, int* totalFileSize, int packetId)
{
    //base64 encoding
    char* encoded_data;
    encoded_data = b64_encode((const unsigned char *)packetString, strlen(packetString));

    char* resultQuery; //data ke kterým přilepím base_host
    int resultLength = strlen(encoded_data) + strlen(BASE_HOST);
    resultQuery = (char*)malloc((resultLength+3)*sizeof(char));
    
    if (!resultQuery)
    {
        printf("Malloc failed\n");
        return -1;
    }

    strcat(resultQuery, encoded_data);
    strcat(resultQuery, ".");
    strcat(resultQuery, BASE_HOST);
	
    dns_sender__on_chunk_encoded(BASE_HOST, packetId, resultQuery);

    //https://www.binarytides.com/dns-query-code-in-c-with-linux-sockets/?fbclid=IwAR0PYjXl9dIjuo3er2CgmPh31zJ2cl6U7twNHAfO3_r3VLzj8EZg5_H7ah4
    unsigned char buf[65536],*qname;
    int i, socketfd;

    //struct sockaddr_in a;
    struct sockaddr_in dest;

    struct DNS_HEADER *dns = NULL;
    struct QUESTION *qinfo = NULL;


    socketfd = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); //UDP packet for DNS queries
    if ( (socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) { 
        perror("socket creation failed"); 
        return 1;
    } 

    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    dest.sin_addr.s_addr = inet_addr(dstAddress); //dns servers


    //Set the DNS structure to standard queries
    dns = (struct DNS_HEADER *)&buf;

    dns->id = (unsigned short) htons(getpid());
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 1; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;

    //point to the query portion
    qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];

    ChangetoDnsNameFormat(qname , (unsigned char*)resultQuery);
    qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; //fill it

    qinfo->qtype = htons( T_A ); //type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1); //its internet (lol)

	*totalFileSize += strlen((const char*)encoded_data);
	dns_sender__on_chunk_sent(&dest.sin_addr, BASE_HOST, packetId, strlen((const char*)encoded_data));

    if( sendto(socketfd,(char*)buf,sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION),0,(struct sockaddr*)&dest,sizeof(dest)) < 0)
    {
        perror("sendto failed");
    } 

    //Receive the answer
    i = sizeof dest;
    unsigned int len;
    char recvMessage[100];

    //answer time interval
    //https://stackoverflow.com/questions/13547721/udp-socket-set-timeout
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 500000;
    if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) 
    {
        perror("Error setting timer");
    }

    //kontrola odeslaného packetu. Pokud přišla jiná data něž byla odeslána, nebo nedošlo v rámci 0.5s žádná odpověď, pošlou se data znovu
    if( recvfrom (socketfd,recvMessage , strlen((char*)qname) , MSG_WAITALL , (struct sockaddr*)&dest , &len ) < 0)
    {
        printf("---no response from sever: resending packet---\n");
        sendPacket(packetString, BASE_HOST, dstAddress, totalFileSize, packetId + 1);
    }
    if(strncmp(encoded_data, recvMessage,strlen(recvMessage)) != 0)
    {
        printf("---wrong answer: resending packet---\n");
        sleep(1);
        sendPacket(packetString, BASE_HOST, dstAddress, totalFileSize, packetId + 1);
    }
    
 
    //reset hodnot pro další paket
    close(socketfd);
    
    memset(recvMessage,0,strlen(recvMessage));
    memset(encoded_data,0,strlen(encoded_data));
    memset(resultQuery,0,strlen(resultQuery));
    free(resultQuery);
    free(encoded_data);
    return 1;
}  