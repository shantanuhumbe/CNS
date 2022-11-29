#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#define MAX 10
int left = 0, right = 0;
struct mesg_buffer
{
    long mesg_type;
    char mesg_text[100];
} message;
static struct sigaction siga;
static void multi_handler(int sig, siginfo_t *siginfo, void *context)
{
    if (sig == SIGUSR2)
    {
        int sender_pid = siginfo->si_pid;
        left = sender_pid;
        kill(right, SIGUSR2);
        printf("left process=%d\n", sender_pid);
    }
    if (sig == SIGUSR1)
    {
        int sender_pid = siginfo->si_pid;
        right = sender_pid;
        kill(left, SIGUSR1);
        printf("right process=%d\n", sender_pid);
    }

    return;
}

int main()
{

    siga.sa_sigaction = *multi_handler;
    siga.sa_flags |= SA_SIGINFO;
    sigaction(SIGUSR1, &siga, NULL);
    sigaction(SIGUSR2, &siga, NULL);
    key_t key;
    int msgid;
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 2;
    sprintf(message.mesg_text, "%d", getpid());
    msgsnd(msgid, &message, sizeof(message), 2);
    printf("Data send is : %s \n", message.mesg_text);
    while (message.mesg_type != 3)
    {
        msgrcv(msgid, &message, sizeof(message), 3, 3);
    }

    left = atoi(message.mesg_text);

    while (1)
        ;

    return 0;
}
