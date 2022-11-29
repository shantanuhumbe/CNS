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
//char *ip = "127.0.0.1";
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
int recv_fd(int socket)
{
    int sent_fd, available_ancillary_element_buffer_space;
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    /* start clean */
    memset(&socket_message, 0, sizeof(struct msghdr));
    memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));
    /* setup a place to fill in message contents */
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;
    /* provide space for the ancillary data */
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = CMSG_SPACE(sizeof(int));
    if (recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
    {
        // printf("\n1\n");
        return -1;
    }
    // printf("%s", message_buffer);
    if (message_buffer[0] != 'F')
    {
        /* this did not originate from the above function */
        // printf("\n2\n");
        return -1;
    }
    if ((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
    {

        /* we did not provide enough space for the ancillary element array */
        // printf("\n3\n");
        return -1;
    }
    /* iterate ancillary elements */
    for (control_message = CMSG_FIRSTHDR(&socket_message);
         control_message != NULL;
         control_message = CMSG_NXTHDR(&socket_message, control_message))
    {
        if ((control_message->cmsg_level == SOL_SOCKET) &&
            (control_message->cmsg_type == SCM_RIGHTS))
        {
            sent_fd = *((int *)CMSG_DATA(control_message));
            return sent_fd;
        }
    }
    return -1;
}

int mkusfd_server()
{
    int usfd;
    struct sockaddr_un userv_addr, ucli_addr;
    int userv_len, ucli_len;
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    perror("socket");
    // printf("%d", usfd);
    bzero(&userv_addr, sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    unlink(ADDRESS);
    userv_len = sizeof(userv_addr);
    if (bind(usfd, (struct sockaddr *)&userv_addr, userv_len) == -1)
        perror("server: bind");
    listen(usfd, 5);
    ucli_len = sizeof(ucli_addr);
    socklen_t addr_size = sizeof(ucli_addr);
    int nusfd = accept(usfd, (struct sockaddr *)&ucli_addr, &addr_size);
    return nusfd;
}
int mksfd(int port)
{
    int sfd;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sfd, 10);
    return sfd;
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
    char * data; 
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    char buff[]="Movie got released\n";
    int n;
     socklen_t len = sizeof(sin) ;
	strcpy(data ,buff);
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("\n\t Warning: I was not able to set HDRINCL!\n");
      int k=47;
      for(int i=0;i<2;i++)
      {
        iph->ip_p=k;
        sendto (s,datagram,iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));
        k++;
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


    printf("This is producer\n");
    int usfd=mkusfd_server();
    int cnt=0;
    while(1)
    {
        char buf[100];
        for(int i=0;i<6;i++)
        {
        int sfd=recv_fd(usfd);
        char buf[]="Got_paid_from_producer\n";
        send(sfd,buf,sizeof(buf),0);
        sleep(1);
        }
        printf("sent movie to the screens\n");
        informer();
    }
}