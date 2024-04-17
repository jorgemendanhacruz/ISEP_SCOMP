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

    int arr1[20];
    int i;
    for (i = 0; i < 20; i++) {
        arr1[i] = rand() % 10; // Generates random numbers between 0 and 9
    }

    int arr2[20];
    for (i = 0; i < 20; i++) {
        arr1[i] = rand() % 10; // Generates random numbers between 0 and 9
    }



    int arr_to_fill[20];
    int min = 0;
    int max = 4;

    enum read_write
    {
        READ = 0,
        WRITE = 1
    };


    int pipes[5][2];
    for(i = 0; i < 5; i++){
        if(pipe(pipes[i]) == -1){
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid;

    int j;
    for (i = 0; i < 5; i++)
    {

        pid = fork();
        if (pid == -1)
        {
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            close(pipes[i][READ]);

            printf("I am the child process number %d. I will sum from %d to %d\n", i + 1, min, max);

            int arr_temp[20];

            for (j = min; j < max; j++)
            {
                arr_temp[j] = arr1[j] + arr2[2];
                printf("%d + %d = %d\n",arr1[j], arr2[j], arr_temp[j]);
            }

            write(pipes[i][WRITE], &arr_temp, sizeof(arr_temp));
            close(pipes[i][WRITE]);

            exit(EXIT_SUCCESS);
        }

        if (pid > 0)
        {
            close(pipes[i][WRITE]);

            int arr_temp2[20];

            read(pipes[i][READ], &arr_temp2, sizeof(arr_temp2));
            close(pipes[i][READ]);

            printf("I am the father process. In iterarion %d, i recieved values from %d to %d\n", i + 1, min, max);

            for (j = min; j < max; j++)
            {
                arr_to_fill[j] = arr_temp2[j];
                printf("[%d] - %d | ", j, arr_to_fill[j]);
            }
            printf("\n");
            
        }

        max += 4;
        min += 4;
    }

    return 0;
}