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

    data_1->num1 = 10000;
    data_1->num2 = 500;
    data_1->new_data = 0;

    pid = fork();
    if (pid == -1)
    {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        int i;

        for (i = 0; i < 1000000; i++)
        {
            data_1->num1--;
            data_1->num2++;
        }

        data_1->new_data = 1;
        wait(NULL);

        printf("num1 : %d\n", data_1->num1);
        printf("num2 : %d\n", data_1->num2);

        munmap(data_1, DATA_SIZE);
        shm_unlink(SHM_NAME);

        exit(EXIT_SUCCESS);
    }
    else
    {
        int i;
        while (!data_1->new_data);       

        for (i = 0; i < 1000000; i++)
        {
            data_1->num1++;
            data_1->num2--;
        }

        data_1->new_data = 1;

        exit(EXIT_SUCCESS);
    }
    return 0;
}
