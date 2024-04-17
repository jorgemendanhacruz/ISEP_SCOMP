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
#define NUMBER_OF_PROCESSES 0

// Macros for pipes
#define READ 0
#define WRITE 1

// Other global variables

// #define VAR1 value
// #define VAR2 value

// Function to create child processes
int createChild(int n)
{
    printf("Creating child processes...\n");

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
            printf("I am the child %d. I was successfully created.\n", i + 1);
            return i + 1;
        }
    }
    // All child processes returned. Father process code
    int father_id = 0;
    printf("I am the father. My ID is %d. I was successfully created.\n", father_id);
    return father_id;
}



//Code for father process
void father_function()
{
    printf("Father is saying hello and goodbye\n");
    exit(EXIT_SUCCESS);
}


//Code for child processes
void son_function(int id)
{
    printf("Son %d is saying hello and goodbye\n", id);
    exit(EXIT_SUCCESS);
}



int main()
{
    int number_of_child = 4;

    int id = createChild(number_of_child);

    //Father processes end the function with id 0
    if (id == 0)
    {
        father_function(id);
    }

    //Child processes 
    else
    {
        son_function(id);
    }

    return 0;
}