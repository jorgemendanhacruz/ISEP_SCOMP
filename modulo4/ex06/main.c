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
#include "data.h"

#define CHILDS 5;

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
    const int DATA_SIZE = sizeof(data);
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, DATA_SIZE);
    data *data_1 = (data *)mmap(NULL, DATA_SIZE,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);


    int n = 5;
    int arr[5];

    int id = createChild(n);

    if (id != 0)
    {
        if (id > 1)
        {
            while (!arr[id - 1]);          ;
        }

        printf("I am the child number %d. Insert your word.\n", id);

        char new_word[100];
        scanf("%99s", new_word);

        // Append the new word to the existing phrase
        strcat(data_1->phrase, new_word);

        // Now you can use child_data.phrase to access the updated string
        printf("Updated phrase: %s\n", data_1->phrase);

        arr[id - 1] = 1;
        int count_terminated = 0;

        int i;
        for (i = 0; i < n; i++)
        {
            if (arr[i] == 1)
            {
                count_terminated++;
            }
        }

        if (count_terminated == n)
        {
            data_1->new_data = 1;
        }

        munmap(data_1, DATA_SIZE);
        close(fd);

        exit(EXIT_SUCCESS);
    }

    else
    {
        wait(NULL);

        printf("I am the father. The complete phrase is: %s\n", data_1->phrase);

        munmap(data_1, DATA_SIZE);
        close(fd);
        shm_unlink(SHM_NAME);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
