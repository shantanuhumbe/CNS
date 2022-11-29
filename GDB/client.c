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
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "p1.cpp";
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    fp = fopen(filename, "r");
    char data[SIZE] = {0};
    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            exit(1);
        }
        bzero(data, SIZE);
    }
    close(sockfd);
    char arr1[100];
    int fd;
    char myfifo[] = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_RDONLY);
    read(fd, arr1, sizeof(arr1));
    printf("%s", arr1);
    return 0;
}