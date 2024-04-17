#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    pid = fork();
    pid = fork();

    int i;
    for (i = 0; i < 5; i++)
    {
        pid = fork();

        execlp(" exam ", " exam ", NULL);

        if (pid == 0)
            break;
    }

    execlp(" students ", " students ", " scomp ", NULL);
}