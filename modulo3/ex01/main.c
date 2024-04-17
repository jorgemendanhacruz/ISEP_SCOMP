#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>



int main()
{
    enum extremidade{READ=0, WRITE=1};

    int fd[2] = {0};
    if(pipe(fd) == -1){
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid == -1){
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if(pid > 0){
        close(fd[READ]);
        int father_pid = getpid();
        printf("I am the father process. My pid is: %d\n", getpid());
        write(fd[WRITE], &father_pid, sizeof(father_pid));
        close(fd[WRITE]);
        exit(EXIT_SUCCESS);
    }

    if(pid == 0){
        close(fd[WRITE]);
        int father_pid;
        read(fd[READ], &father_pid, sizeof(father_pid));
        printf("I am the child process. The pid of my father is %d.\n", father_pid);
        close(fd[READ]);
        exit(EXIT_SUCCESS);
    }

    

    return 0;
}
