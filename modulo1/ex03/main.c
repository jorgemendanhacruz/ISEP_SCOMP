#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int i;

    for (i = 0; i < 4; i++){
        pid = fork();
    }
    printf(" Computer Systems\n");
    return 0;
}
