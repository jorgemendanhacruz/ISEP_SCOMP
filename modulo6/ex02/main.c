#include <stdio.h>
#include <pthread.h>

#define QTD_THREADS 1

void* primeNumbers(void* arg)
{
    int value = *((int*) arg);
    printf("I am the thread. Prime numbers up to %d are:\n", value);
    for (int num = 2; num <= value; num++) {
        int isPrime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
            printf("%d ", num);
        }
    }
    printf("\n");
    
    pthread_exit((void *)NULL);
}

int main()
{
    pthread_t threads[QTD_THREADS] = {0};

    printf("Type the greatest value: \n");
    int number;
    scanf("%d", &number);

    int i;
    for (i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, primeNumbers, (void*)&number);
    }


    for (i = 0; i < QTD_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
