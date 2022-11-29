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
int actor[10][10],sets[10][10];
int ac[10];
int st[10];
char *name[10];
int usfd;
int psfd;
int mkusfd_client()
{
    
    struct sockaddr_un userv_addr;
    int userv_len, ucli_len;
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    bzero(&userv_addr, sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    userv_len = sizeof(userv_addr);
    while (connect(usfd, (struct sockaddr *)&userv_addr, userv_len) == -1)
    {
        // perror("\n not connect ");
        // return 0;
    }
    printf("connected\n");

    return usfd;
}
int send_fd(int socket, int fd_to_send)
{
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    /* storage space needed for an ancillary element with a paylod of
   length is CMSG_SPACE(sizeof(length)) */
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    int available_ancillary_element_buffer_space;
    /* at least one vector of one byte must be sent */
    message_buffer[0] = 'F';
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;
    /* initialize socket message */
    memset(&socket_message, 0, sizeof(struct msghdr));
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;
    /* provide space for the ancillary data */
    available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
    memset(ancillary_element_buffer, 0,
           available_ancillary_element_buffer_space);
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen =
        available_ancillary_element_buffer_space;
    /* initialize a single ancillary data element for fd passing */
    control_message = CMSG_FIRSTHDR(&socket_message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
    *((int *)CMSG_DATA(control_message)) = fd_to_send;
    return sendmsg(socket, &socket_message, 0);
}
void send_to_producer(int k)
{
  
   // send(psfd,name[k],sizeof(name[k]),0);
  for(int i=0;i<ac[k];i++)
  {
    send_fd(usfd,actor[k][i]);
  }
  for(int i=0;i<st[k];i++)
  {
    send_fd(usfd,sets[k][i]);
  }

}
void connect_actor(int k)
{

  for(int i=0;i<ac[k];i++)
  {
     int port = 8080;
    int e;
    port=port+actor[k][i];
    int sfd1;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size = sizeof(server_addr);
    sfd1 = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    actor[k][i]=sfd1;
    char buf[100];
    recv(sfd1,buf,50, 0);
    printf("%s\n",buf);
  }
  
}
void connect_set(int k)
{
  for(int i=0;i<st[k];i++)
  {
     int port = 8090;
    int e;
    port=port+sets[k][i];
    int sfd1;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size = sizeof(server_addr);
    sfd1 = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    sets[k][i]=sfd1;
  }
}
void decode_packet(unsigned char *buf, size_t length,int n,int k) {
    buf+=40;
    char msg[40];
    memset(msg,'\0',sizeof(msg));
    int l=0;
    for(int i=0;i<40;i++)
    {
        if(buf[i]>=32&&buf[i]<=128)
        {
            msg[l++]=buf[i];
        }
        else
        {
          break;
        }
    }
  printf("Length of the string %d\n",strlen(msg));
    if(n==0)
    {
          printf("Connecting to the Actors and Settings for the Script %s\n",msg);
    }
    if(n==1)
    {
      printf("\nSelected sets are:-\n");
      for(int i=0;i<strlen(msg);i++)
      {
          sets[k][st[k]++]=(msg[i]-48);
          printf("Set%d\n",sets[k][st[k]-1]);
      }
      connect_set(k);
    }
    if(n==2)
    {
      printf("Selected actors are:-\n");
      for(int i=0;i<strlen(msg);i++)
      {
          actor[k][ac[k]++]=(msg[i]-48);
          printf("Actor%d\n",actor[k][ac[k]-1]);
      }
      connect_actor(k);
    }
    //printf("%s\n",msg);
}
int prot;
void* get_all(void * k_desc)
{

     int k = *(int *)k_desc;
    int s = socket (PF_INET, SOCK_RAW, 40);	

       // printf("%d\n",s);
    int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");

  char datagram[4096];	

 
 // struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);

  struct sockaddr_in sin,sin2; socklen_t len = sizeof(sin2) ;

memset(&sin,0,sizeof(sin)) ;

  sin.sin_family = AF_INET;
  sin.sin_port = htons (P);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");

  bind(s,(struct sockaddr *) &sin,sizeof(sin)) ;
 
   // while(1)
   // {
        for(int i=0;i<3;i++)
        {

        if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	            printf ("error\n");

        decode_packet(datagram,n,i,k);

        }
        send_to_producer(k);
       
  //  }
}

int main()
{


  printf("This is the Director\n");
  usfd=mkusfd_client();
  int k=0;
  printf("%d\n",psfd);
  printf("Enter protocol if new script to be accepted\n");
  while(1)
  {           
            char buf[10];
            scanf("%s",buf);
            pthread_t sniffer_thread;
             int *new_sock;
            new_sock = malloc(1);
            *new_sock = k;
            if (pthread_create(&sniffer_thread, NULL, get_all,  (void *)new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            k++;
  }
  while(1);
    return 0;
}