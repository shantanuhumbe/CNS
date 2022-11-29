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
char *ip = "127.0.0.1";
int client_fd1[5], client_fd2[5];
int i1 = 0, i2 = 0;
int mksfd(int port)
{
    int sfd;
    struct sockaddr_in server_addr;
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
void *connection_handler1(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    int n;

    char client_message[2000];

    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {
        printf("This is info from server s1 \n");
        printf("%s\n", client_message);
        printf("Tranferring above info to the client who choose server s1\n");
        for (int i = 0; i < i1; i++)
        {
            send(client_fd1[i], client_message, strlen(client_message), 0);
        }
        bzero(client_message, sizeof(client_message));
    }
    close(sock);
    return 0;
}
void *connection_handler2(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    int n;

    char client_message[2000];

    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {
        printf("This is info from server s2 \n");
        printf("%s\n", client_message);
        printf("Tranferring above info to the client who choose server s2\n");
        for (int i = 0; i < i2; i++)
        {
            send(client_fd2[i], client_message, strlen(client_message), 0);
        }
        bzero(client_message, 2000);
    }
    close(sock);
    return 0;
}

int main()
{

    printf("This is mediated server\n");
    int port = 8080;
    int e;

    int sfd1;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size = sizeof(server_addr);
    sfd1 = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    int *new_sock;
    pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = sfd1;
    if (pthread_create(&sniffer_thread, NULL, connection_handler1, (void *)new_sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }
    int sfd2;
    port = 8081;
    sfd2 = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd2, (struct sockaddr *)&server_addr, sizeof(server_addr));
    new_sock = malloc(1);
    *new_sock = sfd2;
    if (pthread_create(&sniffer_thread, NULL, connection_handler2, (void *)new_sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }
    struct pollfd pfds[3];
    port = 8091;
    for (int i = 0; i < 3; i++)
    {
        pfds[i].fd = mksfd(port);
        pfds[i].events = POLLIN;
        port++;
    }
    while (1)
    {

        int ret = poll(pfds, 10, -1);
        if (ret == -1)
        {
            perror("poll");
            exit(1);
        }
        int flg = 0;
        for (int i = 0; i < 10; i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                flg = i;
                int nsfd = accept(pfds[i].fd, (struct sockaddr *)&new_addr, &addr_size);
                if (nsfd == -1)
                {
                    perror("accept");
                    exit(1);
                }
                else
                {
                    printf("Accepted connection from client\n");
                }
                char buf[100];
                recv(nsfd, buf, 10, 0);
                if (buf[1] == '1')
                {
                    client_fd1[i1++] = nsfd;
                    printf("Alloted special server s1\n");
                }
                else
                {
                    client_fd2[i2++] = nsfd;
                    printf("Alloted special server s2\n");
                }
                break;
            }
        }
    }
    return 0;
}