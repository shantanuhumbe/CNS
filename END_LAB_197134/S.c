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
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MSG_CMSG_CLOEXEC
#define MSG_CMSG_CLOEXEC 0
// we explicitly close all file descriptors obtained thought the recv_fd anyway
#endif
#define __USE_BSD	
#include <sys/socket.h>	
#include <netinet/in.h>	
#include <netinet/ip.h>
#define __FAVOR_BSD	
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#define O_RDWR 02
#define P 9013		
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
unsigned short		
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}
void decode_packet(unsigned char *buf, size_t length) {
    buf+=40;
    char msg[40];
    memset(msg,'\0',sizeof(msg));
    int k=0;
    for(int i=0;i<40;i++)
    {
        if(buf[i]>=32&&buf[i]<=128)
        {
            msg[k++]=buf[i];
        }
    }
    printf("%s\n",msg);
}
void inform_all(char datagram[])
{
    int s = socket (PF_INET, SOCK_RAW, 40);	
       printf("%d\n",s);
  struct ip *iph = (struct ip *) datagram;
  //  struct tcpheader *tcp = (struct tcpheader *) (datagram + sizeof(struct ip));
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");
    int one = 1;
    const int *val = &one;
    while(1)
    {
        for(int i=0;i<3;i++)
        {
            char * data; 
            data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
            char buff[100];
            int n;
            socklen_t len = sizeof(sin) ;
            if(i==0)
            {
                printf("Enter the script name to send:-\n");
            }
            if(i==2)
            {
                printf("Enter the actor ids as continuous string to send:-\n");
            }
            if(i==1)
            {
                printf("Enter the Set numbers to send:-\n");
            }
            scanf("%s",buff);
	        strcpy(data ,buff);
            if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
            printf ("\n\t Warning: I was not able to set HDRINCL!\n");
            int k=40;
            iph->ip_p=k;
            sendto (s,datagram,iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));
        }

    }
}
void informer()
{
      int s = socket (PF_INET, SOCK_RAW, 45);	
  char datagram[4096];	
     printf("%d\n",s);
  struct ip *iph = (struct ip *) datagram;
  //  struct tcpheader *tcp = (struct tcpheader *) (datagram + sizeof(struct ip));
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");

  memset(&datagram,0,4096);	
  iph->ip_hl = 5;
  iph->ip_v = 4;
  iph->ip_tos = 0;
  iph->ip_len =sizeof (struct ip);
  iph->ip_id = htonl (54321);	
  iph->ip_off = 0;
  iph->ip_ttl =255;
  iph->ip_p = 45;
  iph->ip_sum = 0;		
  iph->ip_src.s_addr = inet_addr ("1.2.3.4");
  iph->ip_dst.s_addr = sin.sin_addr.s_addr;

  iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);

    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("\n\t Warning: I was not able to set HDRINCL!\n");

        socklen_t len = sizeof(sin) ;
        int n=0;
        sendto (s,datagram,iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));
         if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	        printf ("error\n");

       inform_all(datagram);


}

int main()
{
    printf("This is the Script writer\n");
    informer();
  

}