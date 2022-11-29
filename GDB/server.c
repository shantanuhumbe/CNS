#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SIZE 1024
int cmpr(FILE *fPtr1, FILE *fPtr2, int *line, int *col)
{
    char ch1, ch2;
    *line = 1;
    *col = 0;
    do
    {
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);
        if (ch1 == '\n')
        {
            *line += 1;
            *col = 0;
        }
        if (ch1 != ch2)
            return -1;

        *col += 1;

    } while (ch1 != EOF && ch2 != EOF);
    if (ch1 == EOF && ch2 == EOF)
        return 0;
    else
        return -1;
}
int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;
    int sfd, nsfd;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sfd, 10);
    addr_size = sizeof(new_addr);
    nsfd = accept(sfd, (struct sockaddr *)&new_addr, &addr_size);
    int n;
    FILE *fp;
    char *filename = "p2.cpp";
    fp = fopen(filename, "w");
    while (1)
    {
        n = recv(nsfd, buffer, SIZE, 0);
        if (n <= 0)
        {
            break;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    fclose(fp);
    int c = fork();
    if (c > 0)
    {
        wait(&c);
        FILE *f1, *f2;
        f1 = fopen("Pout.txt", "r");
        f2 = fopen("OT.txt", "r");
        int line, col;
        int fd;
        char myfifo[] = "/tmp/myfifo";
        mkfifo(myfifo, 0666);
        fd = open(myfifo, O_WRONLY);
        if (cmpr(f1, f2, &line, &col) == 0)
        {
            char out[] = "1_test_case_passed \n";
            write(fd, out, strlen(out) + 1);
        }
        else
        {
            char out[] = "1_test_case_failed \n";
            write(fd, out, strlen(out) + 1);
        }
    }
    else
    {
        int itfd, poutfd;
        itfd = open("IT.txt", O_RDONLY);
        poutfd = open("Pout.txt", O_WRONLY | O_CREAT, 0644);
        dup2(itfd, 0);
        dup2(poutfd, 1);
        char *my_args[2];
        my_args[0] = "p2";
        my_args[1] = NULL;
        system(" g++ -std=c++17 p2.cpp -o p2 ");
        int k = execv("./p2", NULL);
        printf("%d", k);
    }
}