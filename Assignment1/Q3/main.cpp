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
    char str3[] = "Message from p1 to p3";
    write(pfd1[1], str3, strlen(str3) + 1);
    close(pfd1[1]);
    wait(&c);
    char str[100];
    read(pfd1[0], str, 100);
    printf("%s\n", str);
  }
  else
  {
    stdin = pfd1[1];
    dup2(pfd1[0], 0);
    char str[100];
    sprintf(str, "%d", stdin);
    char *const parmList[] = {str, NULL};
    execv("./p2", parmList);
  }
  return 0;
}