#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>
const char *semName = "asdfsd";
int main()
{
    sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 0);
    if (sem_id == SEM_FAILED)
    {
        perror("[sem_open] Failed\n");
        return 1;
    }
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char *)shmat(shmid, (void *)0, 0);
    int t;
    sem_getvalue(sem_id, &t);
    printf("%d\n", t);
    while (1)
    {
        sem_wait(sem_id);
        strcat(str, "p2");
        printf("modified data: %s\n", str);
        sleep(2);
        sem_post(sem_id);
    }
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
