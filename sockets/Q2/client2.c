#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
int main()
{
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9040);
  server_address.sin_addr.s_addr = INADDR_ANY;
  int conn = connect(sfd, (struct sockaddr *)&server_address, sizeof(server_address));

  if (conn == -1)
  {
    printf("cant connect\n");
  }
  char sbuff[256], rbuff[256];
  int MAX_SIZE = 255;

  while (fgets(sbuff, MAX_SIZE, stdin) != NULL)
  {
    send(sfd, sbuff, strlen(sbuff), 0);

    if (recv(sfd, rbuff, MAX_SIZE, 0) == 0)
      printf("Error");
    else
      fputs(rbuff, stdout);

    bzero(rbuff, MAX_SIZE);
  }

  return 0;
}