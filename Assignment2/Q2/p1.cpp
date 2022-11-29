#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd1;
    char myfifo[] = "/tmp/myfifo";
    char myfifo1[] = "/tmp/myfifo1";
    mkfifo(myfifo, 0666);
    mkfifo(myfifo1, 0666);

    char str1[80], str2[80];
    while (1)
    {
        fd1 = open(myfifo, O_RDONLY);
        read(fd1, str1, 80);
        printf("Client message: %s", str1);
        fd1 = open(myfifo1, O_WRONLY);
        fgets(str2, 80, stdin);
        write(fd1, str2, strlen(str2) + 1);
    }
    return 0;
}
