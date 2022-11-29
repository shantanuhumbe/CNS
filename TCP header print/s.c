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

    char *data;
        struct ip *iph = (struct ip *) buf;
        struct tcphdr *tcph = (struct tcphdr *) buf+ sizeof (struct ip);
	data =buf + sizeof(struct iphdr) + sizeof(struct tcphdr);
    data+=20; 
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
  int s = socket (PF_INET, SOCK_RAW, IPPROTO_UDP);	
  char datagram[512];	
  
  struct ip *iph = (struct ip *) datagram;

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");

  memset(&datagram,0,512);	
  iph->ip_hl = 5;
  iph->ip_v = 4;
  iph->ip_tos = 0;
  iph->ip_len =sizeof (struct ip);
  iph->ip_id = htonl (54321);	
  iph->ip_off = 0;
  iph->ip_ttl =255;
  iph->ip_p = 10;
  iph->ip_sum = 0;		
  iph->ip_src.s_addr = inet_addr ("1.2.3.4");
  iph->ip_dst.s_addr = sin.sin_addr.s_addr;

  iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("\n\t Warning: I was not able to set HDRINCL!\n");


         struct sockaddr_in sin2; socklen_t len = sizeof(sin2) ;
      sendto (s,datagram,iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));
      int n;
        if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
            {}
            decode_packet(datagram,n);
  while(1);
  return 0;
}

