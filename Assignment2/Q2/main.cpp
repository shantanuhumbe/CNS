#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd;

    char myfifo[] = "/tmp/myfifo";
    char myfifo1[] = "/tmp/myfifo1";
    mkfifo(myfifo, 0666);
    mkfifo(myfifo1, 0666);
    char arr1[80], arr2[80];
    while (1)
    {

        fd = open(myfifo, O_WRONLY);
        fgets(arr2, 80, stdin);
        write(fd, arr2, strlen(arr2) + 1);
        fd = open(myfifo1, O_RDONLY);
        read(fd, arr1, sizeof(arr1));
        printf("user message: %s", arr1);
    }
    return 0;
}