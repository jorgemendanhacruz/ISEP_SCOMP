#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


#define BUFFER_SIZE 80

int main()
{
    enum values{READ=0, WRITE=1};
    int fd[2] = {0};
    
    if(pipe(fd) == -1){
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("Fork failure");
        exit(EXIT_FAILURE);
    }

    if(pid > 0){
        close(fd[READ]);

        printf("I am the father process.\n");

        printf("Insert a number\n");  
        int number;
        scanf("%d", &number);

        

        printf("Insert a name\n");  
        char name[10];
        scanf("%10s", name);

        write(fd[WRITE], &number, sizeof(number));
        write(fd[WRITE], name, sizeof(name));

        close(fd[WRITE]);

        exit(EXIT_SUCCESS);

    }

    if(pid == 0){
        close(fd[WRITE]);

        char name[10];
        int number;

        read(fd[READ], &number, sizeof(number));
        read(fd[READ], name, sizeof(name));


        printf("I am the child process.\n");
        printf("The number sent by father is %d.\n", number);
        printf("The name sent by father is %10s.\n", name);


        close(fd[READ]);

        exit(EXIT_SUCCESS);

    }


    

    return 0;
}
