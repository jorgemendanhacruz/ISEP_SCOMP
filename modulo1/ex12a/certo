#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    int data[20];

    int i;
    for (i = 0; i < 20; i++)
    {
        data[i] = rand() % 10;
    }
    printf("Array inicial: \n");
    for (i = 0; i < 20; i++)
    {
        printf("Data [%d]: %d\n", i, data[i]);
    }
    printf("\n");

    int result[20];
    for (i = 0; i < 20; i++)
    {
        result[i] = 0;
    }

    pid = fork();

    if (pid == -1)
    {
        printf("Fork failure\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        wait(NULL);

        for (i = 10; i < 20; i++)
        {
            result[i] = data[i] * 4 + 20;
            printf("Result na posição [%d] %d\n", i, result[i]);
        }
        exit(EXIT_SUCCESS);
    }

    if (pid == 0)
    {
        for (i = 0; i < 10; i++)
        {
            result[i] = data[i] * 4 + 20;
            printf("Result na posição [%d] %d\n", i, result[i]);
        }
        sleep(5);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
