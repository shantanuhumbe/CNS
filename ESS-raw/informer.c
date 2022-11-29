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

static void decode_packet(unsigned char *buf, size_t length, FILE *lf);
static void print_content(unsigned char *buf, size_t length, FILE *lf);

void sigint_handler() {
  
    exit(0);
}
#define P 9013	
void send_fil(char datagram[],int k)
{
    
   
    int s = socket (PF_INET, SOCK_RAW,k);	
    struct ip *iph = (struct ip *) datagram;
    struct sockaddr_in sin;
			
    iph->ip_p=k;
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

int 
main (void)
{
    printf("This is batsmen\n");
 int s = socket (PF_INET, SOCK_RAW,45);	


int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");

  char datagram[4096];	

  struct ip *iph = (struct ip *) datagram;

  struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);

  struct sockaddr_in sin,sin2; socklen_t len = sizeof(sin2) ;

memset(&sin,0,sizeof(sin)) ;

  sin.sin_family = AF_INET;
  sin.sin_port = htons (P);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s,(struct sockaddr *) &sin,sizeof(sin)) ;
 
   
   while(1)
   {
      if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
        {}
       // send_ump(datagram);
        int k=45;
        send_fil(datagram,k);
   }
        while(1);
  return 0;
}

