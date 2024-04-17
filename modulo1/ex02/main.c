#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t p;
    int i;

    for (i = 0; i < 2; i++)
    {
        p = fork();
        if (p == -1)
        {
            printf("Fork failure\n");
            exit(EXIT_FAILURE);
        }
        if (p > 0)
        {
            printf("I am the father. My pid is %d\n", getpid());
            int status = 0;
            pid_t pid_2 = waitpid(p, &status, 0);
            if (WIFEXITED(status))
            {
                printf("Father: the son %d returned the value %d\n", pid_2, WEXITSTATUS(status));
            }
            printf("\n");
        }
        if (i == 0)
        {
            if (p == 0)
            {
                printf("I am the first child. My pid is %d\n", getpid());
                sleep(5);
                exit(1);
            }
        }
        if (i == 1)
        {
            if (p == 0)
            {
                printf("I am the second child. My pid is %d\n", getpid());
                exit(2);
            }
        }
    }
    return 0;
}
