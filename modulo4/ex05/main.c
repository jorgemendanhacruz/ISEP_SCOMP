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

    pid_t pid;

    int v[1000];
    int i;
    for (i = 0; i < 1000; i++)
    {
        v[i] = rand();
        // printf("%d", v[i]);
    }

    if (pid == -1)
    {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    }

    int id = createChild(10);

    if (id != 0)
    {
        int range_min = (id - 1) * 100;
        int range_max = range_min + 100;

        // printf("I am the child num %d. I will look between %d and %d.\n", id, range_min, range_max);

        int max = 0;
        int i;
        for (i = range_min; i < range_max; i++)
        {
            if (v[i] > max)
            {
                max = v[i];
            }
        }

        printf("I am the child num %d. Max is %d.\n", id, max);

        data_1->arr[id - 1] = max;
        data_1->terminated[id - 1] = 1;

        int count_terminated = 0;
        for (i = 0; i < 10; i++)
        {
            if (data_1->terminated[i] == 1)
            {
                count_terminated++;
            }
        }

        if (count_terminated == 10)
        {
            data_1->new_data = 1;
        }

        munmap(data_1, DATA_SIZE);
        close(fd);

        exit(EXIT_SUCCESS);
    }

    else
    {
        int i;
        int absolute_greater = 0;

        while (!data_1->new_data)
            ;

        for (i = 0; i < 10; i++)
        {
            if (data_1->arr[i] > absolute_greater)
            {
                absolute_greater = data_1->arr[0];
            }
        }

        printf("I am the father. The winner ir %d.\n", absolute_greater);

        munmap(data_1, DATA_SIZE);
        close(fd);
        shm_unlink(SHM_NAME);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
