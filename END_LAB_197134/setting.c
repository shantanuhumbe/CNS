#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#define SIZE 1024;
char *ip = "127.0.0.1";
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
void *connection_handler(void *socket_desc)
{


    printf("This is Set \n");
    int sock = *(int *)socket_desc;
    int n;
     char buf[]="Accepted";
    send(sock, buf, sizeof(buf), 0);
    char client_message[2000];
    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {
        printf("%s\n", client_message);
        bzero(client_message, sizeof(client_message));
        break;
    }
    close(sock);
    return 0;
}
int main()
{


    int set[10],set_cnt[10];
    for(int i=0;i<10;i++)
    {
        set_cnt[i]=0;
    }
     int port = 8090;
    int e;
    int sfd, nsfd;
    struct sockaddr_in new_addr;
    socklen_t addr_size = sizeof(new_addr);
    for(int i=0;i<10;i++)
    {
        set[i]= mksfd(port);
        port++;
    }



    struct pollfd pfds[10];
    for(int i=0;i<10;i++)
    {
        pfds[i].fd=set[i];
        pfds[i].events=POLLIN;

    }


    while(1)
    {
        int ret=poll(pfds,10,-1);
        if(ret==-1)
        {
            perror("poll");
            return -1;
        }
        printf("Hellon\n");
        for(int i=0;i<10;i++)
        {
            if(pfds[i].revents&POLLIN)
            {
                int nsfd=accept(pfds[i].fd,(struct sockaddr *)&new_addr,&addr_size);
                set_cnt[i]++;
                //if(set_cnt[i]>1)
               // {
                    printf("Set%d is already booked by %d no. of scripts\n",i,set_cnt[i]);
                      pthread_t sniffer_thread;
                    int *new_sock;
                     new_sock = malloc(1);
                    *new_sock = nsfd;
                    if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
                    {
                        perror("could not create thread");
                        // return 1;
                    }
               // }
                break;
            }
        }
    }
 /*   nsfd = accept(sfd, (struct sockaddr *)&new_addr, &addr_size);
    if (nsfd == -1)
    {
        perror("accept");
        exit(1);
    }
    while (1)
    {
        char buf[50];
        scanf("%s", buf);
        send(nsfd, buf, sizeof(buf), 0);
    }*/
}