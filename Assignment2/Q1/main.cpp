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
    FILE *fd;

    int orig = dup(1);
    fd = popen("./p2", "w");
    string s = "hello_from_process_p1";
    char buf[100];
    for (int i = 0; i < s.length(); i++)
    {
        buf[i] = s[i];
    }
    buf[s.length()] = '\0';
    int k = fileno(fd);
    fprintf(fd, "%s", buf);
    //dup2(k, 1);
    fclose(fd);
    int fdr;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fdr = open(myfifo, O_RDONLY);
    char but[100];
    read(fdr, buf, 100);
    string str;
    for (int i = 0; i < strlen(buf); i++)
    {
        str.push_back(buf[i]);
    }
    str += "\n message back in process p1\n";
    cout << str;
    return 0;
}