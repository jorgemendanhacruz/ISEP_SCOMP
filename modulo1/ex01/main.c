#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
    pid_t p, a;
    p = fork();
    if(p == -1){
        printf("Fork failed in process p.");
        exit(EXIT_FAILURE);
    }
    a = fork();
    if(a == -1){
        printf("Fork failed in process a.");
        exit(EXIT_FAILURE);
    }
    printf(" Computer Systems \n");

    return 0;
}
