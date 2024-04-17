#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int i;

    for (i = 0; i < 3; i++)
    {
        pid = fork();

        if (pid == 0)
            pid = fork();

        if (pid > 0)
            execlp(" scomp ", " scomp ", NULL);
            //faz exec e morre
    }
}