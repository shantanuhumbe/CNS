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
int main()
{

    printf("This is special server s1\n");
    int port = 8080;
    int e;
    int sfd, nsfd;
    struct sockaddr_in new_addr;
    socklen_t addr_size = sizeof(new_addr);
    sfd = mksfd(port);
    nsfd = accept(sfd, (struct sockaddr *)&new_addr, &addr_size);
    if (nsfd == -1)
    {
        perror("accept");
        exit(1);
    }
    printf("Connected to mediated server\n");
    printf("Type info to forward it to client who chosen server1\n");
    while (1)
    {
        char buf[1024];
        scanf("%s", buf);
        send(nsfd, buf, sizeof(buf), 0);
    }
}