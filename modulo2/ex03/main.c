#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void handle_SIGSEGV(){
    write(STDOUT_FILENO, "\nI got signal SIGSEV\n", 22);
    exit(0);
}


int main()
{

    struct sigaction act;
    memset(&act, 0, sizeof(sigaction));

    act.sa_handler = handle_SIGSEGV;
    sigaction(SIGSEGV, &act, NULL);



    int a;
    a = *(int *)0;
}