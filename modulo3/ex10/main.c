#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>

// Number of used processes
#define NUMBER_OF_CHILDS 5

// Macros for pipes
#define READ 0
#define WRITE 1

// Other global variables

// Function to create child processes
int createChild(int n)
{

    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            return i + 1;
        }
    }
    // All child processes returned. Father process code
    int father_id = 0;
    return father_id;
}

// Code for father process
void father_function(int fd[])
{
}

// Code for child processes
void child_function(int fd[])
{
}

int main()
{
    int fd[6][2];
    int i, greater;
    for (i = 0; i < 6; i++)
    {
        if (pipe(fd) == -1)
        {
            perror("Pipe failed.");
        }
    }

    int id = createChild(NUMBER_OF_CHILDS);
    int num;

    for (i = 0; i <= NUMBER_OF_CHILDS; i++)
    {
        if (id == i)
        {
            num = (rand() % 500) + 1;
            printf("My pid is %d. Generated number %d\n", getpid(), num);
        }

        // Seed the random number generator with the process ID
        srand(getpid());
    }

    for (i = 0; i <= NUMBER_OF_CHILDS; i++)
    {
        if (id == 0)
        {
            
            write(fd[0][WRITE], &num, sizeof(num));
            close(fd[0][WRITE]);
        }
        else
        {
            int recieved;
            
            read(fd[i - 1][READ], &recieved, sizeof(int));
            close(fd[i - 1][READ]);

            if (recieved > num)
            {
                write(fd[i][WRITE], &recieved, sizeof(int));
                close(fd[i][WRITE]);
            }
            else
            {
                write(fd[i][WRITE], &num, sizeof(int));
                close(fd[i][WRITE]);
            }

            exit(EXIT_SUCCESS);
        }
    }
    read(fd[5][READ], &greater, sizeof(int));
    close(fd[READ]);
    printf("The greater is %d\n", greater);

    return 0;
}