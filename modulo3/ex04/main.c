#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>

#define BUFFER_SIZE 20

void capitalize(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (islower(str[i]))
        {
            str[i] = toupper(str[i]);
        }
        else if (isupper(str[i]))
        {
            str[i] = tolower(str[i]);
        }
    }
}

int main()
{
    enum read_write
    {
        READ = 0,
        WRITE = 1
    };
    int fd_up[2] = {0};
    int fd_down[2] = {0};

    if (pipe(fd_down) == -1 || pipe(fd_up) == -1)
    {
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        close(fd_up[READ]);
        close(fd_down[WRITE]);

        char buffer[BUFFER_SIZE];
        printf("I am the Client (child process). Insert the message:\n");
        scanf("%s", &buffer);

        write(fd_up[WRITE], buffer, BUFFER_SIZE);
        close(fd_up[WRITE]);

        read(fd_down[READ], buffer, BUFFER_SIZE);
        close(fd_down[READ]);
        printf("Client process: Message treated by server [%s]\n", buffer);

        exit(EXIT_SUCCESS);
    }

    if (pid > 0)
    {
        close(fd_up[WRITE]);
        close(fd_down[READ]);

        char recieved[BUFFER_SIZE];
        read(fd_up[READ], recieved, BUFFER_SIZE);
        close(fd_up[READ]);

        printf("I am the Server (parent process). Capitalizing...\n");
        for (int i = 0; i < 3; i++)
        {
            printf(".");
            fflush(stdout); // Flush stdout to ensure the dots are printed immediately
            sleep(1);
            printf(" .");
            fflush(stdout);
            sleep(1);
            printf(" .\n");
            fflush(stdout);
            sleep(1);
        }

        capitalize(recieved);

        write(fd_down[WRITE], recieved, BUFFER_SIZE);
        close(fd_down[WRITE]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}