#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    int i, inicial, final;

    inicial = 1;
    final = 201;

    for (i = 0; i < 6; i++)
    {

        pid = fork();
        if (pid == -1)
        {
            exit(EXIT_FAILURE);
        }


        if (pid == 0)
        {
            for (i = inicial; i < final; i++)
            {
                printf("%d\n", i);                
            }
            exit(EXIT_SUCCESS);
        }

        inicial += 200;
        final += 200;
    }

    if (pid > 0){        
        exit(EXIT_SUCCESS);
    }

    return 0;
}
