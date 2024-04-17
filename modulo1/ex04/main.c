#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    int f;

    for (f = 0; f < 3; f++)
    {
        pid = fork();
        if (pid > 0)
        {
            printf("I am the father\n");
        }

        else
        {
            sleep(1);
        }
    }

    return 0;
}
