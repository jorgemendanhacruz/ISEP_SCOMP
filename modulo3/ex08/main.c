#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>
#include "record_t.h"

// Number of used processes
#define NUMBER_OF_PROCESSES 10

// Macros for pipes
#define READ 0
#define WRITE 1

// Other useful macros
#define RECORDS 50000
#define SEARCH 5000
#define GOAL 20

// Function to create child processes
int create_child(int n)
{
    //("Creating child processes...\n");
    // printf("\n");

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
            // printf("I am the child %d. I was successfully created.\n", i + 1);
            return i + 1;
        }
    }
    // All child processes returned. Father process code
    int father_id = 0;
    // printf("I am the father. My ID is %d. I was successfully created.\n", father_id);
    return father_id;
}

// Code for father process
void father_function(int fd[2])
{
    close(fd[WRITE]);

    int larger[RECORDS];
    int product_code;

    int i = 0;

    while (read(fd[READ], &product_code, sizeof(int)) > 0)
    {
        larger[i++] = product_code;
    }

    for (i = 0; i < RECORDS; i++)
    {
        if (larger[i] > 0)
        {
            printf("Product code: %d\n ", larger[i]);
        }
        else
        {
            //printf("Not greater than 20: %d\n ", larger[i]);
        }
    }

    close(fd[READ]);

    exit(EXIT_SUCCESS);
}

// Code for child processes
void son_function(int id, int fd[2], record_t *records)
{
    close(fd[READ]);

    int range_min = (id - 1) * SEARCH;
    int range_max = range_min + SEARCH;
    printf("I am the child numbe %d. I will look between %d and %d.\n", id, range_min, range_max);

    // Search
    int i;
    for (i = range_min; i < range_max; i++)
    {
        if (records[i].quantity > GOAL)
        {
            //printf("Child: PRODUCT CODE -> %d | QUANTITY -> %d", records[i].product_code, records[i].quantity);
            write(fd[WRITE], &records[i].product_code, sizeof(int));
        }
    }

    close(fd[WRITE]);

    exit(EXIT_SUCCESS);
}

void create_records(record_t *records, int n)
{

    int i;
    for (i = 0; i < n; i++)
    {
        records[i].customer_code = i + 1;
        records[i].product_code = (rand() % 10) + 1;
        records[i].quantity = rand() % 50;

        // Data creation validation
        /*if (i == 49000)
        {
            printf("All data created\n");
            printf("\n");
        }*/
    }
}

int main()
{

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    }

    record_t records[RECORDS];
    create_records(records, RECORDS);

    int id = create_child(NUMBER_OF_PROCESSES);

    // Father processes end the function with id 0
    if (id == 0)
    {
        father_function(fd);
    }

    // Child processes
    else
    {
        son_function(id, fd, records);
    }

    return 0;
}