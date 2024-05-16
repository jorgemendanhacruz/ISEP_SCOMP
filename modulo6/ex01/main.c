#include <stdio.h>
#include <pthread.h>

#define QTD_THREADS 2

void* displayString(void* arg)
{
    char* str = (char*) arg;
    printf("%s\n", str);
    pthread_exit((void *)NULL);
}

int main()
{
    pthread_t threads[QTD_THREADS] = {0};

    char str0[] = "Jorge";
    char str1[] = "Cruz";

    int i;
    for (i = 0; i < QTD_THREADS; i++)
    {
        char* str = (i == 0) ? str0 : str1;
        pthread_create(&threads[i], NULL, displayString, (void*)str);
    }

    for (i = 0; i < QTD_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
