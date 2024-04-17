#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


#define BUFFER_SIZE 80


struct message{
    int number;
    char name[10];
};

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
        struct message sending;

        printf("I am the father process.\n");

        printf("Insert a number\n");  
        scanf("%d", &sending.number);

        

        printf("Insert a name\n");  
        scanf("%10s", sending.name);

        write(fd[WRITE], &sending, sizeof(sending));

        close(fd[WRITE]);

        exit(EXIT_SUCCESS);

    }

    if(pid == 0){
        close(fd[WRITE]);

        struct message recieving;

        read(fd[READ], &recieving, sizeof(recieving));


        printf("I am the child process.\n");
        printf("The number sent by father is %d.\n", recieving.number);
        printf("The name sent by father is %10s.\n", recieving.name);


        close(fd[READ]);

        exit(EXIT_SUCCESS);

    }


    

    return 0;
}
