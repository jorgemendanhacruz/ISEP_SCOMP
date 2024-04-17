#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>

int main()
{
    struct data
    {
        char str[4];
        int round;
    };

    struct data game;
    strcpy(game.str, "Win!");
    
    printf("RACE TO PIPE\n");
    printf("\n");
    
    enum read_write
    {
        READ = 0, 
        WRITE = 1
    };

    int fd[2];
    if(pipe(fd) == -1){
        perror("Pipe failed\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    int i;
    for(i = 0; i < 10; i++){
        if(pid > 0){
            close(fd[READ]);
            game.round = i;

            write(fd[WRITE], &game, sizeof(game));
        }
    }

    

    return 0;
}