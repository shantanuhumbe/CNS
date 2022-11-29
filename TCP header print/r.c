#define __USE_BSD	
#include <sys/socket.h>	
#include <netinet/in.h>	
#include <netinet/ip.h>
#define __FAVOR_BSD	
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <netinet/tcp.h> 
#include <netinet/udp.h> 
#include <netinet/ip_icmp.h> 
#include <netinet/ip.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <signal.h> 
#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <netinet/tcp.h> 
#include <netinet/udp.h> 
#include <netinet/ip_icmp.h>
#include <netinet/ip.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <signal.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <pthread.h>
#define SIZE 1024
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#define ADDRESS "mysocket"
char *ip = "127.0.0.1";
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MSG_CMSG_CLOEXEC
#define MSG_CMSG_CLOEXEC 0
#endif


#define BUF_SIZE 65536



void sigint_handler() {
  
    exit(0);
}
#define P 9013	
void send_fil(char datagram[])
{
    
    int s = socket (PF_INET, SOCK_RAW,IPPROTO_UDP);	
    struct ip *iph = (struct ip *) datagram;
    struct sockaddr_in sin;
			
    iph->ip_p=10;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");
      if (sendto (s,		
		  datagram,	
		  iph->ip_len,	
		  0,		
		  (struct sockaddr *) &sin,	
		  sizeof (sin)) > 0)		
      sendto (s,datagram,	iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));

}
void decode_packet(unsigned char *buf, size_t length) {
    char *data;
        struct ip *iph = (struct ip *) buf;
        struct tcphdr *tcph = (struct tcphdr *) buf+ sizeof (struct ip);
	data =buf + sizeof(struct iphdr) + sizeof(struct tcphdr);
    data+=20;
    printf("Hello\n");  
    char datag[50];
    memset(datag,'\0',sizeof(datag));
    int i=0;
    while(data[i] >= 32 && data[i] <= 128)
    {
        datag[i]=data[i];
        i++;
    }
    printf("%s\n",datag);
    struct iphdr *ip_head = (struct iphdr*)buf;
    struct sockaddr_in ip_source, ip_dest;
    unsigned short ip_head_len = ip_head->ihl*4;
    static int packet_count = 0;

    memset(&ip_source, 0, sizeof(ip_source));
    memset(&ip_dest, 0, sizeof(ip_dest));
    ip_source.sin_addr.s_addr = ip_head->saddr; // Get source IP address
    ip_dest.sin_addr.s_addr = ip_head->daddr; // Get destination IP address
    struct tcphdr *tcp_head = (struct tcphdr*)(buf + ip_head_len);

    printf( "\nTCP header\n");
    printf( "   Source port: %u\n", ntohs(tcp_head->source));
    printf( "   Destination port: %u\n", ntohs(tcp_head->dest));
    printf( "   Sequence number: %u\n", ntohl(tcp_head->seq));
    printf( "   Ack number: %u\n", ntohl(tcp_head->ack_seq));
    printf( "   Header length: %u Bytes\n", (unsigned int)tcp_head->doff*4);
    printf( "   UFLAG: %u\n", (unsigned int)tcp_head->urg);
    printf( "   AFLAG: %u\n", (unsigned int)tcp_head->ack);
    printf( "   PFLAG: %u\n", (unsigned int)tcp_head->psh);
    printf( "   RFLAG: %u\n", (unsigned int)tcp_head->rst);
    printf( "   SFLAG: %u\n", (unsigned int)tcp_head->syn);
    printf( "   FFLAG: %u\n", (unsigned int)tcp_head->fin);
    printf( "   Window: %u\n", htons(tcp_head->window));
    printf("   Checksum: %u\n", htons(tcp_head->check));
    printf( "   urgent Pointer: %u\n", htons(tcp_head->urg_ptr));
}
int 
main (void)
{
 
 int s = socket (PF_INET, SOCK_RAW, 10);	


int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");

  char datagram[4096];	

  

  struct sockaddr_in sin,sin2; socklen_t len = sizeof(sin2) ;

memset(&sin,0,sizeof(sin)) ;

  sin.sin_family = AF_INET;
  sin.sin_port = htons (P);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s,(struct sockaddr *) &sin,sizeof(sin)) ;
 
   
      if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
        {}
        char *data;
        send_fil(datagram);
      // decode_packet(datagram,n);
        while(1);
  return 0;
}

