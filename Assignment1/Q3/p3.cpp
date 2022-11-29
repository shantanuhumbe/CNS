#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char **argv)
{

  char *a = argv[0];
  int stdin = atoi(a);
  char str[100];
  read(0, str, 100);
  printf("%s\n", str);

  char str1[] = "Message from p3 to p1";
  write(stdin, str1, strlen(str1) + 1);

  return 0;
}