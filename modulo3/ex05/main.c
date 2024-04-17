#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>

int main()
{
    enum read_write
    {
        READ = 0,
        WRITE = 1
    };
    int fd[2] = {0};

    if (pipe(fd) == -1)
    {
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    int i, result, obtained;
    int arr1[1000] = {1};
    int arr2[1000] = {1};

    int min = 0;
    int max = 200;

    for (i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Fork failure\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            close(fd[READ]);
            printf("I am the child. I will sum from %d to %d\n", min, max);
            int sum = 0;
            for (i = min; i < max; i++)
            {
                sum += arr1[min] + arr2[min];
            }
            write(fd[WRITE], &sum, sizeof(sum));
            close(fd[WRITE]);
            exit(EXIT_SUCCESS);
        }

        if (pid > 0)
        {

            close(fd[WRITE]);

            read(fd[READ], &obtained, sizeof(obtained));
            close(fd[READ]);

            printf("Obtained in the interation %d: %d\n", i + 1, obtained);
            

            result += obtained;
            wait(NULL);
        }


        min += 200;
        max += 200;
    }

    
    printf("The result is %d\n", result);

    return 0;
}