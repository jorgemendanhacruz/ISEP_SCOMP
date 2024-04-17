#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 80

int main()
{
    enum read_write
    {
        READ = 0,
        WRITE = 1
    };

    int fd[2] = {0};
    if (pipe(fd) == -1)
    {
        perror("pipe failed\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {

        close(fd[READ]);

        FILE *file_ptr;

        char read_file[BUFFER_SIZE];

        file_ptr = fopen("file.txt", "r");
        if (file_ptr == NULL)
        {
            perror("Error opening file\n");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (fgets(read_file, BUFFER_SIZE, file_ptr))
        {
            read_file[i] = file_ptr;
        }
        fclose(file_ptr);

        write(fd[WRITE], read_file, sizeof(read_file));

        close(fd[WRITE]);

        exit(EXIT_SUCCESS);
    }

    if (pid == 0)
    {

        close(fd[WRITE]);

        char read_father[BUFFER_SIZE];

        read(fd[READ], read_father, BUFFER_SIZE);
        printf("I read the following message from father: \n");
        printf("%100s\n", read_father);

        close(fd[READ]);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
