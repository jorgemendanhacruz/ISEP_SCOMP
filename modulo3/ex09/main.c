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
#define NUMBER_OF_CHILDS 1

// Macros for pipes
#define READ 0
#define WRITE 1

// Other global variables
#define CASH 20
#define INCREASE 10
#define DECREASE 5

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
    int ammount = CASH;
    int can_bet, value, bet;
    int i = 1;

    while (1)
    {
        value = (rand() % 5) + 1;
        //printf("FATHER: generated number %d [%d]\n", value, i);

        if (ammount > 0)
        {
            can_bet = 1;
            //printf("FATHER: Make a bet!\n");
            write(fd[WRITE], &can_bet, sizeof(int));
        }
        else
        {
            int can_bet = 0;
            //close(fd[READ]); 
            write(fd[WRITE], &can_bet, sizeof(int));
            //close(fd[WRITE]);
            
            wait(NULL);
            exit(EXIT_SUCCESS);
        }

        read(fd[READ], &bet, sizeof(int));
        if (bet == value)
        {
            ammount += 10;
        }
        else
        {
            ammount -= 5;
        }

        //printf("FATHER: Your ammount is %d [%d]\n" , ammount, i);

        write(fd[WRITE], &ammount, fd[WRITE]);

        i++;

    }
}

// Code for child processes
void child_function(int fd[])
{

    int can_bet;
    int bet, i;
    int ammount = CASH;

    while (1)
    {

        read(fd[READ], &can_bet, sizeof(int));

        if (can_bet == 0)
        {
            //printf("CHILD: No more money [%d]\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            
            bet = (rand() % 5) + 1;
            //printf("CHILD: My bet is %d [%d]\n", bet);
            write(fd[WRITE], &bet, sizeof(int));
        }

        read(fd[READ], &ammount, sizeof(int));
        printf("My ammount is: %d [%d]\n", ammount);
    }

    i++;
}

int main()
{
    int fd[3][2];
    if (pipe(fd) == -1)
    {
        perror("Pipe failed\n");
        exit(EXIT_FAILURE);
    }

    int id = createChild(NUMBER_OF_CHILDS);

    // Father processes end the function with id 0
    if (id == 0)
    {
        father_function(fd);
    }

    // Child processes
    else
    {
        child_function(fd);
    }

    return 0;
}