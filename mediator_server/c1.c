#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SIZE 1024
int main()
{
    printf("This is client1\n");
    char *ip = "127.0.0.1";
    int port = 8091;
    int e;

    int sfd;
    struct sockaddr_in server_addr;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Enter s1 or s2 the server which one you want to connect\n");
    char buf[100];
    scanf("%s", buf);
    send(sfd, buf, strlen(buf), 0);
    printf("You will receive information from chosen server \n");
    while (1)
    {
        recv(sfd, buf, sizeof(buf), 0);
        printf("%s\n", buf);
        bzero(buf, sizeof(buf));
    }
    close(sfd);
    return 0;
}