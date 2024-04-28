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

#define NUMBER_OF_CHILDS 1
#define SHM_NAME "/mod5_ex01"
#define SEM_NAME "/mod5_ex01_sem"

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
    int *word_counter = NULL;
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, sizeof(word_counter));
    word_counter = (int *)mmap(NULL, sizeof(int),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);

    *word_counter = 0;

    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED)
    {
        fprintf(stderr, "%s sem_open() failed!!!\n%s\n", SEM_NAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    int id = createChild(NUMBER_OF_CHILDS);

    if (id != 0)
    {

        sem_wait(sem);

        FILE *fptr = fopen("file.txt", "a");

        // Write some text to the file
        int i;

        fprintf(fptr, "I am child %d.\n", id);
        for (i = 0; i < 20; i++)
        {
            fprintf(fptr, "%d \n", *(word_counter));
            ++(*word_counter);
        }

        // Close the file
        fclose(fptr);

        sem_post(sem);

        if (munmap(word_counter, sizeof(word_counter)) == -1)
        {
            perror("Failed munmap!!!");
            exit(EXIT_FAILURE);
        }
        if (close(fd) == -1)
        {
            perror("Failed close()!!!");
            exit(EXIT_FAILURE);
        }
        if (sem_close(sem) == -1)
        {
            perror("sem_close() failed!!!");
            exit(EXIT_FAILURE);
        }
    }

    else
    {

        wait(NULL);

        FILE *fptr = fopen("file.txt", "r");

        int ch;
        while ((ch = fgetc(fptr)) != EOF)
        {
            putchar(ch);
        }

        fclose(fptr);

        if (shm_unlink(SHM_NAME) == -1)
        {
            perror("shm_unlink() failed!!!");
            exit(EXIT_FAILURE);
        }
        if (sem_unlink(SEM_NAME) == -1)
        {
            perror("sem_unlink() failed!!!");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
