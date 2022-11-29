#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;
int main()
{
	int pfd1[2];
	int pfd2[2];
	pipe(pfd1);
	pipe(pfd2);
	int c;
	c = fork();

	if (c < 0)
	{
		printf("fork failed");
	}
	else if (c > 0)
	{
		char str1[100];

		char str3[] = "Message from parent to child";
		close(pfd2[0]);
		int flag = 1;

		write(pfd2[1], str3, strlen(str3) + 1);

		close(pfd2[1]);

		wait(NULL);

		close(pfd1[1]);

		read(pfd1[0], str1, 100);
		printf("%s\n", str1);

		close(pfd1[0]);
	}
	else
	{
		// wait(NULL);
		char str4[100];
		close(pfd2[1]);

		read(pfd2[0], str4, 100);
		printf("%s\n", str4);

		close(pfd2[0]);

		char str[] = "Message from child to parent";
		close(pfd1[0]);
		write(pfd1[1], str, strlen(str) + 1);

		close(pfd1[1]);

		exit(0);
	}
}
