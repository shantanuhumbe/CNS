#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main()
{
    // char msg[256] = "hello world";

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address, s1, s2;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9025);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(sfd, (struct sockaddr *)&server_address, sizeof(server_address));
    while (1)
    {
        printf("waiting for connection\n");
        getsockname(sfd, (struct sockaddr *)&s1, sizeof(s1));
        printf("the local port is=%d\n", s1.sin_port);
        char *ip = inet_ntoa(s1.sin_addr);
        printf("the local ip is=%s\n", ip);
        listen(sfd, 5);
        int nsfd = accept(sfd, NULL, NULL);
        getpeername(sfd, (struct sockaddr *)&s2, sizeof(s2));
        printf("the foreign port is=%d\n", s2.sin_port);
        char *ip1 = inet_ntoa(s2.sin_addr);
        printf("the foreign ip is=%s\n", ip1);

        char msg[1024];
        recv(nsfd, msg, sizeof(msg), 0);
        printf("%s\n", msg);
    }

    return 0;
}