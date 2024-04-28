#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <string.h>
#include "data.h"

#define NUMBER_OF_CHILDS 5
#define SEM_NOME_MAX_TAM 100

#define SEM_NAME_BASE "/mod5_ex02_sem"

// Create shared memory
//  shm_open() ➔ ftruncate() ➔ mmap()

/*Close:
munmap() ➔ close()

Remove:
munmap() ➔ close() ➔ shm_unlink()
munmap() ➔ shm_unlink()
*/

// To get the desired result, active wait must be implemented

int createChild(int n)
{
    // printf("Creating child processes...\n");

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

int main()
{
    /*Creating shared memory*/
    const int DATA_SIZE = sizeof(data);
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, DATA_SIZE);
    data *data_1 = (data *)mmap(NULL, sizeof(int),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);

    /********************************************************************************************************************/
    /*Creating semaphores*/
    enum index
    {
        SEM_1,
        SEM_2,
        SEM_3,
        SEM_4,
        SEM_5
    };
    int sems_init_values[5] = {1, 0, 0, 0, 0};
    const int QTD_SEMS = sizeof(sems_init_values) / sizeof(sems_init_values[0]);
    char sems_names[QTD_SEMS][SEM_NOME_MAX_TAM];
    memset(sems_names, 0, sizeof(sems_names));

    sem_t *sems[QTD_SEMS];
    int i;
    for (i = 0; i < QTD_SEMS; ++i)
    {
        snprintf(sems_names[i], SEM_NOME_MAX_TAM, "%s%d", SEM_NAME_BASE, i);
        if ((sems[i] = sem_open(sems_names[i], O_CREAT | O_EXCL, 0644, sems_init_values[i])) == SEM_FAILED)
        {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
    }

    /********************************************************************************************************************/
    /*Creating child processes*/

    int id = createChild(NUMBER_OF_CHILDS);

    /********************************************************************************************************************/
    /*Child processes code*/

    if (id != 0)
    {
        sem_wait(sems[id - 1]);

        FILE *fptr = fopen("file.txt", "a");

        // Write some text to the file
        int i;

        fprintf(fptr, "I am child %d.\n", id);
        fprintf(fptr, "I am the process with PID %d.\n", getpid());

        sleep(2);

        // Close the file
        fclose(fptr);

        if (id != NUMBER_OF_CHILDS)
        {
            sem_post(sems[id]);
        }

        exit(EXIT_SUCCESS);
    }

    for (i = 0; i < NUMBER_OF_CHILDS; i++)
    {
        wait(NULL);
    }

    printf("\n");
    printf("Father process code\n");
    printf("Print file\n");

    FILE *fptr = fopen("file.txt", "r");

    int ch;
    while ((ch = fgetc(fptr)) != EOF)
    {
        putchar(ch);
    }

    for (i = 0; i < QTD_SEMS; i++)
    {
        if (sem_close(sems[i]) == -1)
        {
            perror("sem_close");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < QTD_SEMS; i++)
    {
        if (sem_unlink(sems_names[i]) == -1)
        {
            perror("sem_close");
            exit(EXIT_FAILURE);
        }
    }

    if (shm_unlink(SHM_NAME) == -1)
    {
        perror("shm unlink failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}
