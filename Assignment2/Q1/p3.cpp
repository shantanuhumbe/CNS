#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>
#include <filesystem>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
int main()
{

    string str;
    cin >> str;
    //cout << str << endl;
    str += "\n message in process p3";
    //cout << str << endl;
    int fd;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);
    int n = str.length();
    char buf[n + 1];
    for (int i = 0; i < n; i++)
    {
        if (str[i] >= 97 && str[i] <= 122)
        {
            buf[i] = str[i] - 32;
        }
        else
        {
            buf[i] = str[i];
        }
    }
    buf[n] = '\0';
    write(fd, buf, strlen(buf) + 1);
    return 0;
}