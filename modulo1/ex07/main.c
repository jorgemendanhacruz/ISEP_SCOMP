#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void M(char c)
{
    printf("%c\n", c);
}

int main(void)
{
    pid_t pid;

    int i = 0;
    for (i = 0; i < 2; i++)
    {

        pid = fork();
        if (pid == -1)
        {
            printf("Error failure.");
            exit(EXIT_FAILURE);
        }

        if (pid > 0)
        {
            M('A');
        }

        if (pid == 0)
        {
            M('B');

            pid = fork();
            if (pid == -1)
            {
                printf("Error failure.");
                exit(EXIT_FAILURE);
            }

            if (pid > 0)
            {
                M('A');
                return (EXIT_SUCCESS);
            }
        }
    }

    return 0;
}
