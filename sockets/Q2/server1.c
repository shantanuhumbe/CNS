#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
void *connection_handler(void *socket_desc)
{

  printf("connection handler1\n");
  int sock = *(int *)socket_desc;
  int n;

  char sendBuff[100], client_message[2000];

  while ((n = recv(sock, client_message, 2000, 0)) > 0)
  {

    printf("%s", client_message);
    // char inf[] = "got_message";
    // send(sock, inf, 11, 0);
  }
  close(sock);
  return 0;
}
void *connection_handler1(void *socket_desc)
{

  printf("connection_handler2\n");
  int sock = *(int *)socket_desc;
  int n;

  char sendBuff[100], client_message[2000];

  while ((n = recv(sock, client_message, 2000, 0)) > 0)
  {

    printf("%s", client_message);
    // char inf[] = "got_message";
    int k = strlen(client_message);
    for (int i = 0; i < k; i++)
    {
      if (client_message[i] >= 97 && client_message[i] <= 122)
      {
        client_message[i] = client_message[i] - 32;
      }
    }
    send(sock, client_message, k + 1, 0);
  }
  close(sock);
  return 0;
}
void *connection_handler2(void *socket_desc)
{

  printf("connection_handler3\n");
  int sock = *(int *)socket_desc;
  int n;

  char sendBuff[100], client_message[2000];

  while ((n = recv(sock, client_message, 2000, 0)) > 0)
  {

    printf("%s", client_message);
    int k = strlen(client_message);
    for (int i = 0; i < k; i++)
    {
      if (client_message[i] >= 65 && client_message[i] <= 90)
      {
        client_message[i] = client_message[i] + 32;
      }
    }
    send(sock, client_message, k, 0);
  }
  close(sock);
  return 0;
}
int main()
{

  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address, client_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9010);
  server_address.sin_addr.s_addr = INADDR_ANY;
  bind(sfd, (struct sockaddr *)&server_address, sizeof(server_address));
  listen(sfd, 5);
  int nsfd;
  int c = sizeof(struct sockaddr_in);
  int *new_sock;
  nsfd = accept(sfd, (struct sockaddr *)&client_address, (socklen_t *)&c);
  pthread_t sniffer_thread;
  new_sock = malloc(1);
  *new_sock = nsfd;
  pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock);
  int sfd1 = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address1, client_address1;
  server_address1.sin_family = AF_INET;
  server_address1.sin_port = htons(9040);
  server_address1.sin_addr.s_addr = INADDR_ANY;
  bind(sfd1, (struct sockaddr *)&server_address1, sizeof(server_address1));
  listen(sfd1, 5);
  int nsfd1;
  int c1 = sizeof(struct sockaddr_in);
  int *new_sock1;
  nsfd1 = accept(sfd1, (struct sockaddr *)&client_address1, (socklen_t *)&c);
  pthread_t sniffer_thread1;
  new_sock1 = malloc(1);
  *new_sock1 = nsfd1;
  pthread_create(&sniffer_thread1, NULL, connection_handler1, (void *)new_sock1);
  int sfd2 = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address2, client_address2;
  server_address2.sin_family = AF_INET;
  server_address2.sin_port = htons(9050);
  server_address2.sin_addr.s_addr = INADDR_ANY;

  bind(sfd2, (struct sockaddr *)&server_address2, sizeof(server_address2));
  listen(sfd2, 5);
  int nsfd2;
  int c2 = sizeof(struct sockaddr_in);
  int *new_sock2;
  nsfd2 = accept(sfd2, (struct sockaddr *)&client_address2, (socklen_t *)&c2);
  pthread_t sniffer_thread2;
  new_sock2 = malloc(1);
  *new_sock2 = nsfd2;
  pthread_create(&sniffer_thread2, NULL, connection_handler2, (void *)new_sock2);

  while (1)
    ;

  return 0;
}