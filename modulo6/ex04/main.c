#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define QTD_THREADS 5
#define ARR_ELEMENTS 1000
#define NUMBER_TO_BE_FOUND 1700

/* global variables shared by threads */
int data_arr[ARR_ELEMENTS];
int search_value = NUMBER_TO_BE_FOUND;

/* function executed by each thread */
void *search_in_arr(void *arg)
{
    int value = *((int *)arg);
    int range_min = value * (ARR_ELEMENTS / QTD_THREADS);
    int range_max = range_min + (ARR_ELEMENTS / QTD_THREADS);

    printf("I am thread %d. I will look from %d to %d.\n", value, range_min, range_max);

    int i;
    for (i = range_min; i < range_max; i++)
    {
        if (data_arr[i] == search_value)
        {
            printf("Found by thread %d at position %d.\n", value, i);
            int *return_value = (int *)malloc(sizeof(int));
            if (return_value == NULL)
            {
                fprintf(stderr, "Error: malloc failed\n");
                pthread_exit((void *)NULL);
            }
            *return_value = value;
            pthread_exit((void *)return_value);
        }
    }

    pthread_exit((void *)NULL);
}

void initialize_arr(int *arr)
{
    int i;
    int init_value = 1000;

    for (i = 0; i < ARR_ELEMENTS; i++)
    {
        arr[i] = init_value + i;
    }
}

int main()
{
    initialize_arr(data_arr);

    int args[QTD_THREADS];

    pthread_t threads[QTD_THREADS] = {0};

    int i;
    for (i = 0; i < QTD_THREADS; i++)
    {
        args[i] = i;
        pthread_create(&threads[i], NULL, search_in_arr, (void *)&args[i]);
    }

    for (i = 0; i < QTD_THREADS; i++)
    {
        int *value;
        pthread_join(threads[i], (void **)&value);
        if (value != NULL)
        {

            printf("\n");
            printf("MAIN THREAD:\n");
            printf("    Congrats thread %d, you found it!\n", *value);
        }

        free(value);
    }

    return 0;
}
