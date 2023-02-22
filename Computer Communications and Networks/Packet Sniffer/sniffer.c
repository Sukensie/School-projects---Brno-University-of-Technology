// Knihovny pro jednodušší práci
#include <stdio.h>      // printf()
#include <string.h>     // strcpy(), strcat()
#include <stdlib.h>     // atoi()
#include <time.h>       // localtime()


// Síťové knihovny
#include <pcap.h> 
#include <arpa/inet.h>          // inet_ntop()
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <net/if_arp.h>
#include <net/ethernet.h>       //ethernet sktruktura
#include <netinet/ip_icmp.h>	//icmp sktruktura
#include <netinet/udp.h>	    //udp sktruktura
#include <netinet/tcp.h>	    //tcp sktruktura
#include <netinet/ip.h>	        //ip sktruktura

#define IPv6_HDR_SIZE 40 //velikost ipv6 headeru je fixní 40


/* ---------------------------- */
/*        POMOCNÉ FUNKCE        */
/* ---------------------------- */

//výpis dat na stdout
//https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
void PrintData (const u_char * data , int Size)
{
	int i , j, lineCnt = 0;
    printf("0x0000: ");
	for(i=0 ; i < Size ; i++)
	{
		if( i!=0 && i%16==0)   //pokud je dokončený řádek hex výpisu
		{
			printf("         ");
			for(j=i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
					printf("%c",(unsigned char)data[j]); //pokud se jedná o číslo nebo znak abecedy
				
				else printf(".");
			}
            lineCnt += 10;
			printf("\n0x%04d: ", lineCnt);
		} 
		
		if(i%16==0) printf("   ");
			printf(" %02X",(unsigned int)data[i]);
				
		if( i==Size-1)  //konečné mezery
		{
			for(j=0;j<15-i%16;j++) 
			{
			  printf("   "); //mezery navíc
			}
			
			printf("         ");
			
			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) 
				{
				  printf("%c",(unsigned char)data[j]);
				}
				else 
				{
				  printf(".");
				}
			}
			
			printf( "\n" );
		}
	}
}

//výpis IPv4 hlavičky
void printIpHeader(const unsigned char* packet)
{
    const struct ip *ip;
    char ip4src[INET_ADDRSTRLEN];
    char ip4dst[INET_ADDRSTRLEN];

    ip = (struct ip*)(packet + sizeof(struct ether_header));
    inet_ntop(AF_INET, &(ip->ip_src), ip4src, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->ip_dst), ip4dst, INET_ADDRSTRLEN);

    printf("src IP: %s\n", ip4src);
    printf("dst IP: %s\n", ip4dst);
   
}

//výpis tcp packetu
void printTcpPacket(const unsigned char* packet, int size, int version)
{
    //printf("----TCPv%d----\n", version);

    int ipOffset;
    if(version == 4)
    {
        printIpHeader(packet);
        ipOffset = sizeof(struct ip);
    }
    else
    {
        ipOffset = IPv6_HDR_SIZE;
    }
    

    const struct tcphdr *tcp;
    
    tcp = (struct tcphdr*)(packet + sizeof(struct ether_header) + ipOffset);
    int srcPort = ntohs(tcp->source);
    int dstPort = ntohs(tcp->dest);

    printf("src port: %u\n", srcPort);
    printf("dst port: %u\n", dstPort);
    
			
	printf("\n");	
    PrintData(packet + ipOffset + sizeof(struct ether_header), size - ipOffset - sizeof(struct ether_header));

    printf("\n\n-------------------------------------\n\n\n\n");
						
}

//výpis udp packetu
void printUdpPacket(const unsigned char *packet , int size, int version)
{
    //printf("----UDP----\n");
	
    int ipOffset;
    if(version == 4)
    {
        printIpHeader(packet);
        ipOffset = sizeof(struct ip);
    }
    else
    {
        ipOffset = IPv6_HDR_SIZE;
    }
	
	struct udphdr *udph = (struct udphdr*)(packet + ipOffset + sizeof(struct ether_header));
			
	
	printf("src port: %d\n" , ntohs(udph->source));
	printf("dst port: %d\n" , ntohs(udph->dest));
		
	printf("\n");	
	PrintData(packet + ipOffset + sizeof udph +  sizeof(struct ether_header),( size - sizeof udph - ipOffset  - sizeof(struct ether_header)));


    printf("\n\n-------------------------------------\n\n\n\n");
}

//výpis icmp packetu
void printIcmpPacket(const unsigned char* packet , int size, int version)
{
    //printf("----ICMP----\n");

    int ipOffset;
    if(version == 4)
    {
        printIpHeader(packet);
        ipOffset = sizeof(struct ip);
    }
    else
    {
        ipOffset = IPv6_HDR_SIZE;
    }

	
	struct icmphdr *icmph = (struct icmphdr *)(packet + ipOffset + sizeof(struct ether_header));
    
		
	printf("\n");	
	PrintData(packet + ipOffset + sizeof icmph +  sizeof(struct ether_header) + 8, ( size - sizeof icmph - ipOffset - sizeof(struct ether_header) - 8));


    printf("\n\n-------------------------------------\n\n\n\n");
}

//rozhodnutí o jaký protokol v IPv4 se jedná
void processIpv4(const u_char *packet, unsigned int size)
{
    const struct iphdr *ipv4Header = (struct iphdr*) (packet + sizeof(struct ethhdr)); 

    switch(ipv4Header->protocol)
    {
        case 1:  //ICMP - nemá porty
			printIcmpPacket(packet,size, 4);
			break;
		
		case 6:  //TCP
			printTcpPacket(packet , size, 4);
			break;
		
		case 17: //UDP
			printUdpPacket(packet , size, 4);
			break;
    }
}

//rozhodnutí o jaký protokol v IPv6 se jedná a vypsání IP adres
void processIpv6(const u_char *packet, unsigned int size)
{
    const struct ip6_hdr *ipv6Header = (struct ip6_hdr*) (packet+ sizeof(struct ethhdr));

    //printf("src IP: %s\n", ipv6Header->ip6src.__in6_u.__u6_addr32);
    //printf("dst IP: %s\n", ipv6Header->ip6dst.__in6_u.__u6_addr32);
    char ip6src[INET6_ADDRSTRLEN];
    char ip6dst[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET6, &(ipv6Header->ip6_src), ip6src, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &(ipv6Header->ip6_dst), ip6dst, INET6_ADDRSTRLEN);
    printf("src IP: %s\n", ip6src);
    printf("dst IP: %s\n", ip6dst);


    //podle informaci o dalším headeru rozhodne, zda se jedná o TCP, UDP, nebo ICMPv6
    switch(ipv6Header->ip6_ctlun.ip6_un1.ip6_un1_nxt)
    {		
		case 6:  //TCP Protocol
			printTcpPacket(packet, size, 6);
			break;
		
		case 17: //UDP Protocol
			printUdpPacket(packet, size, 6);
			break;

        case 58:  //ICMPv6 Protocol
			printIcmpPacket(packet, size, 6);
			break;
    }
}

//zpracování arp protokolu
void  processArp(const u_char *packet)
{
    //printf("------ARP------\n");

    const u_char *arp_header = packet + sizeof(struct ethhdr); //nenašel jsem vhodnou strukturu, tak jsem to udělal jen jako pointer do paměti
    char ip4src[INET_ADDRSTRLEN];
    char ip4dst[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, arp_header + 14, ip4src, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, arp_header + 24, ip4dst, INET_ADDRSTRLEN);
    printf("src IP: %s\n", ip4src);
    printf("dst IP: %s\n", ip4dst);

 printf("\n\n-------------------------------------\n\n\n\n");
   
}

//prvotní zpracování packetu a rozhodnutí mezi IPv4, IPv6 a ARP, výpis MAC adres, timestamp a délky paketu
void processPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    unsigned int size = header->caplen;
    const struct ethhdr *ethernetHeader = (struct ethhdr*) packet;
    unsigned int etherType = ntohs(ethernetHeader->h_proto); //převedení bytů na správné pořadí

    char buffer [100], timestamp [100];

    struct tm *tm;
    if((tm = localtime(&header->ts.tv_sec)) != NULL)
    {
        strftime(buffer, 100, "%Y-%m-%dT%H:%M:%S.%%06u%z", tm);
        snprintf(timestamp, 100, buffer, &header->ts.tv_sec); 
        printf("timestamp: %s\n", timestamp);
    }
  

    printf("src MAC:  %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", ethernetHeader->h_source[0], ethernetHeader->h_source[1], ethernetHeader->h_source[2], ethernetHeader->h_source[3], ethernetHeader->h_source[4], ethernetHeader->h_source[5]);
    printf("dst MAC:  %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", ethernetHeader->h_dest[0], ethernetHeader->h_dest[1], ethernetHeader->h_dest[2], ethernetHeader->h_dest[3], ethernetHeader->h_dest[4], ethernetHeader->h_dest[5]);
    printf("frame length: %d bytes\n", size);


    if(etherType == ETHERTYPE_IP) 
    {
        processIpv4(packet, size);
    } 
    else if(etherType == ETHERTYPE_IPV6) 
    {
        processIpv6(packet, size);
    }
    else if(etherType == ETHERTYPE_ARP)
    {
        processArp(packet);
    }
}



/* ---------------------------- */
/*           PROGRAM            */
/* ---------------------------- */
int main(int argc, char *argv[])
{

    //https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/

    int argCounter = 0;
    char* interface = malloc(sizeof(char*));
    int numOfPackets = 1; //pokud není zadán -n argument, zobrazí pouze 1 packet
    char *filterExpression = malloc(sizeof(char*)); //sem nahrát, jaký protokol, port atd chci sniffovat

    
    //pcap proměnné
	struct bpf_program filter;
	const u_char *packet;		
    pcap_if_t *alldevsp , *device;
	pcap_t *handle;

	char errbuf[100];


    while(argCounter < argc)
    {
        if (strcmp(argv[argCounter], "-i") == 0 || strcmp(argv[argCounter], "--interface") == 0) 
        {
            if(argCounter + 1 < argc)
            {
                strcpy(interface, argv[argCounter+1]);
            }
            argCounter++;
        } 
        else if (strcmp(argv[argCounter], "-p") == 0) 
        {
            if(argCounter + 1 < argc)
            {
                if(strlen(filterExpression) < 1)
                {
                    strcpy(filterExpression, "port ");
                    strcpy(filterExpression, argv[argCounter+1]);
                }
                else
                {
                    strcat(filterExpression, " and port ");
                    strcat(filterExpression , argv[argCounter+1]);
                }
            }
            argCounter++;
        } 
        else if (strcmp(argv[argCounter], "--tcp") == 0 || strcmp(argv[argCounter], "--udp") == 0 || strcmp(argv[argCounter], "--arp") == 0 || strcmp(argv[argCounter], "--icmp") == 0)
        {
            char result[10];
            strcpy(result, argv[argCounter]+2); //odstranění prvních dvou znaků (--)
            if(strlen(filterExpression) < 1)
            {
                strcpy(filterExpression, result);
                if(strcmp(argv[argCounter], "--icmp") == 0)
                {
                    strcat(filterExpression, " or icmp6");//icmp pro IPv6 se chová narozdíl od IPv6 TCP, UDP, ... jako samostaný protokol -> proto jej uvádím explicitně
                }
            }
            else
            {
                strcat(filterExpression, " or ");
                strcat(filterExpression , result);
                if(strcmp(argv[argCounter], "--icmp") == 0)
                {
                    strcat(filterExpression, " or icmp6");
                }
            }
        }
        else if(strcmp(argv[argCounter], "-t") == 0)
        {
            if(strlen(filterExpression) < 1)
            {
                strcpy(filterExpression, "tcp");
            }
            else
            {
                strcat(filterExpression, " or ");
                strcat(filterExpression , "tcp");
            }
        }
         else if(strcmp(argv[argCounter], "-u") == 0)
        {
            if(strlen(filterExpression) < 1)
            {
                strcpy(filterExpression, "udp");
            }
            else
            {
                strcat(filterExpression, " or ");
                strcat(filterExpression , "udp");
            }
        }
        else if (strcmp(argv[argCounter], "-n") == 0) 
        {
            if(argCounter + 1 < argc)
            {
                numOfPackets = atoi(argv[argCounter+1]);
                argCounter++;
            }
        } 
        argCounter++;
    }

    
    //v případě, že nebylo specifikováno rozhraní
    if(strcmp(interface,"") == 0)
    {
        //najde všechna rozhraní
        pcap_findalldevs( &alldevsp , errbuf);

        //zobrazení aktivních rozhraní
        for(device = alldevsp ; device != NULL ; device = device->next)
        {   
            printf("%s\n" , device->name);
        }
        return 1;
    }
   


	//otevření komunikace se zařízením
	handle = pcap_open_live(interface , 65536 , 1 , 0 , errbuf); //65536 bylo v dokumentaci pcapu, že stačí pro všechny packety; musí být promiskuitní, aby šel provádět packet sniffing; nechci timeout
	
	if (handle == NULL) 
	{
		fprintf(stderr, "Couldn't open device %s : %s\n" , interface , errbuf);
		exit(1);
	}

    if(pcap_datalink(handle) != DLT_EN10MB) 
    {
	    fprintf(stderr, "Device %s doesn't provide Ethernet headers - not supported\n", interface);
	    return(2);
    }  
  
    if(pcap_compile(handle, &filter, filterExpression, 0, PCAP_NETMASK_UNKNOWN) == -1) 
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filterExpression, pcap_geterr(handle));
        return(2);
    }

    if (pcap_setfilter(handle, &filter) == -1) 
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filterExpression, pcap_geterr(handle));
        return(2);
    }

    pcap_loop(handle, numOfPackets, processPacket, NULL);
    pcap_close(handle);
}