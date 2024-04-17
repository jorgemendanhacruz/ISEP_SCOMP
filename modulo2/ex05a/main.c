#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void handle_ALRM(int signo, siginfo_t *sinfo, void *context)
{
    write(STDOUT_FILENO, "\nInside handler!\n", 17); /* Imprimir no ecrã 17 chars */
    return 1;
}

int main()
{

    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGUSR1);
    sigaddset(&act.sa_mask, SIGUSR2);

    act.sa_sigaction = handle_ALRM;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &act, NULL);

    char string[10];
    printf("Escreve uma mensagem rápido!\n");
    alarm(10);
    scanf("%c", string);

    printf("Mensagem: %s\n", string);
    return 0;
}