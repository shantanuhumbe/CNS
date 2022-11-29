#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>
#include <filesystem>
using namespace std;
int main(int argc, char **argv)
{
    int c;
    int pfd1[2];
    pipe(pfd1);
    c = fork();
    int stdin;
    int k = dup(0);
    char *arg_list[] = {};
    if (c < 0)
    {
        cout << "failed";
    }
    else if (c > 0)
    {
        c = 0;
        char str3[100];
        read(0, str3, 100);
        write(pfd1[1], str3, strlen(str3) + 1);
        close(pfd1[1]);
        wait(&c);
        char str[100];
        read(pfd1[0], str, 100);
        char *a = argv[0];
        int stdin = atoi(a);
        write(stdin, str, strlen(str) + 1);
        //wait(&c);
    }
    else
    {
        stdin = pfd1[1];
        dup2(pfd1[0], 0);
        char str[100];
        sprintf(str, "%d", stdin);
        char *const parmList[] = {str, NULL};
        execv("./p3", parmList);
    }
    return 0;
}