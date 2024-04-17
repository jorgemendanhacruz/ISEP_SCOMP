#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void handle_SIGUSR1(int signo, siginfo_t* sinfo, void* context)
{
    write(STDOUT_FILENO, "\nHandled SIGUSR1\n", 18);
}

int main()
{
    pid_t pid;

    struct sigaction act;
    memset(&act, 0, sizeof(sigaction));
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGUSR2);


    act.sa_sigaction = handle_SIGUSR1;
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &act ,NULL);

    pid = fork();
    if (pid == -1)
    {
        printf("Fork error\n");
    }

    if (pid > 0)
    {
        sleep(2);
        kill(pid, SIGUSR1);
        kill(pid, SIGUSR2);
    }

    if(pid == 0){
        printf("Filho: \n");
        for(;;){
            pause();
        }
    }

    return 0;
}