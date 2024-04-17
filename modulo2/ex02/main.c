#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void handle_USR1(int signo)
{
    write(STDOUT_FILENO, "\nDivision by zero not allowed\n", 30);
    exit(0);
}

int divide(int a, int b)
{

    struct sigaction act;

    memset(&act, 0, sizeof(sigaction));
    act.sa_handler = handle_USR1;
    sigaction(SIGUSR1, &act, NULL);

    if (b == 0)
    {
        raise(SIGUSR1);
    }

    return a / b;
}

int main()
{
    int result = divide(4,0);
    printf("%d", result);
}