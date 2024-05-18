#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define QTD_THREADS 5

/* global variables shared by threads */
int init_value;
pthread_mutex_t mux;

/* function executed by each thread */
void *writeInFile(void *)
{
    pthread_mutex_lock(&mux);
    FILE *file;

    file = fopen("file.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file for writing!\n");
        return 0;
    }

    int i;
    for (i = 0; i < 200; i++)
    {
        fprintf(file, "%d\n", i + 1);
    }
    fclose(file);
    pthread_mutex_unlock(&mux);

    pthread_exit((void *)NULL);
}

void readFile()
{
    FILE *fptr = fopen("file.txt", "r");

    int ch;
    while ((ch = fgetc(fptr)) != EOF)
    {
        putchar(ch);
    }

    fclose(fptr);
}

int main()
{
    pthread_t threads[QTD_THREADS] = {0};

    int i;
    for (i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, writeInFile, NULL);
    }

    for (i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    readFile();

    return 0;
}
