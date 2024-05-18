#include <stdio.h>
#include <pthread.h>
#include "data.h"

#define QTD_THREADS 5

void *printData(void *arg)
{
    data value = *((data *)arg);

    printf("I am thread %lu\n", pthread_self());
    printf("number: %d\n", value.number);
    printf("name: %s\n", value.name);
    printf("grade: %d\n", value.grade);
    printf("\n");

    pthread_exit((void *)NULL);
}

void initialize_arr(data *arg)
{
    int i;
    char *arr_names[] = {"João", "Ana", "Joaquim", "Paulo", "José"};
    int arr_grades[] = {10, 12, 14, 16, 18};

    for (i = 0; i < QTD_THREADS; i++)
    {
        data data_to_fill;
        data_to_fill.grade = arr_grades[i];
        data_to_fill.name = arr_names[i];
        data_to_fill.number = i + 1;
        arg[i] = data_to_fill;
    }
}

int main()
{
    data data_arr[QTD_THREADS];

    initialize_arr(data_arr);

    pthread_t threads[QTD_THREADS] = {0};

    int i;
    for (i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, printData, (void *)&data_arr[i]);
    }

    for (i = 0; i < QTD_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }


    return 0;
}
