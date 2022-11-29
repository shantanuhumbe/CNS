#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>
#include <filesystem>
using namespace std;
int main()
{

    char buf[100];
    string str;
    cin >> str;
    int t = 0;
    str += "_message_in_process_p2";
    for (int i = 0; i < str.length(); i++)
    {
        buf[t++] = str[i];
    }
    FILE *fd = popen("./p3", "w");
    int k = fileno(fd);
    dup2(k, 1);
    cout << str;
    return 0;
}