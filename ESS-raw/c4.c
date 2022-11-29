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
void decode_packet(unsigned char *buf, size_t length) {

    // Print IP header first
    char *data;
        struct ip *iph = (struct ip *) buf;
        struct tcphdr *tcph = (struct tcphdr *) buf+ sizeof (struct ip);
    struct iphdr *ip_head = (struct iphdr*)buf;
    struct sockaddr_in ip_source, ip_dest;
    unsigned short ip_head_len = ip_head->ihl*4;
  
    // Print TCP header
    struct tcphdr *tcp_head = (struct tcphdr*)(buf + ip_head_len);
    // Print IP header content
   // print_content(buf, ip_head_len, lf); 
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
    int s = socket (PF_INET, SOCK_RAW, 45);	
       printf("%d\n",s);
  struct ip *iph = (struct ip *) datagram;
  //  struct tcpheader *tcp = (struct tcpheader *) (datagram + sizeof(struct ip));
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");
    int one = 1;
    const int *val = &one;
    char * data; 
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(data , "sfd_is_With_C4");
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("\n\t Warning: I was not able to set HDRINCL!\n");
   int k=10;
    for(int i=0;i<4;i++)
    {
        iph->ip_p=k+i*10;
        sendto (s,datagram,iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));

       // printf("Got it back");
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
void *get_all()
{
    int s = socket (PF_INET, SOCK_RAW, 40);	

        printf("%d\n",s);
    int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");

  char datagram[4096];	

 
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
	printf ("error\n");

         struct ip *iph = (struct ip *) datagram;
       
     
         decode_packet(datagram, n);

        //send_ump();
    }
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
    bzero(&userv_addr, sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    unlink(ADDRESS);
    userv_len = sizeof(userv_addr);
    bind(usfd, (struct sockaddr *)&userv_addr, userv_len);
    listen(usfd, 5);
    ucli_len = sizeof(ucli_addr);
    socklen_t addr_size = sizeof(ucli_addr);
    int nusfd = accept(usfd, (struct sockaddr *)&ucli_addr, &addr_size);
    return nusfd;
}
int mkusfd_client()
{
    int usfd;
    struct sockaddr_un userv_addr;
    int userv_len, ucli_len;
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    bzero(&userv_addr, sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    userv_len = sizeof(userv_addr);
    while (connect(usfd, (struct sockaddr *)&userv_addr, userv_len) == -1)
    {
    }
    printf("connected\n");
    return usfd;
}
void *connection_handler(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    int n;

    char client_message[2000];

    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {
        printf("%s\n", client_message);
        bzero(client_message, sizeof(client_message));
    }
    close(sock);
    return 0;
}
int main()
{
    int k;
   // pthread_t sniffer_thread;
    pthread_t sniffer_thread,thr;
    if(pthread_create(&thr,NULL,get_all,NULL))
    {
        perror("could not create thread");
        return 1;
    }
    printf("Enter the choice number when you want to:\n1)request for sfd\n2)release the sfd\n");
    while (1)
    {

        int ch = 0;
        scanf("%d", &ch);
        if (ch == 1)
        {
            int usfd = mkusfd_client();
            k = recv_fd(usfd);
            int *new_sock;
            printf("sfd received:-\n");
            informer();
            new_sock = malloc(1);
            *new_sock = k;
            if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            close(usfd);
        }
        if (ch == 2)
        {
            int nusfd = mkusfd_server();
            pthread_cancel(sniffer_thread);
            send_fd(nusfd, k);
            printf("sfd released\n");
            close(nusfd);
        }
    }
    return 0;
}