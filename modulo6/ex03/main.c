#include <stdio.h>
#include <pthread.h>
#include "data.h"

#define QTD_THREADS 5

void *printData(void *arg)
{
    data value = *((data *)arg);

    printf("number: %d\n", value.number);
    printf("name: %s\n", value.name);
    printf("grade: %d\n", value.grade);
    printf("\n");

    pthread_exit((void *)NULL);
}

int main()
{
    data data_arr[QTD_THREADS];

    data data1;
    data1.number = 1;
    data1.name = "João";
    data1.grade = 10;
    data_arr[0] = data1;

    data data2;
    data1.number = 2;
    data1.name = "Ana";
    data1.grade = 10;
    data_arr[1] = data1;

    data data3;
    data1.number = 3;
    data1.name = "Joaquim";
    data1.grade = 10;
    data_arr[2] = data1;

    data data4;
    data1.number = 4;
    data1.name = "Paulo";
    data1.grade = 10;
    data_arr[3] = data1;

    data data5;
    data1.number = 5;
    data1.name = "José";
    data1.grade = 10;
    data_arr[4] = data1;

    pthread_t threads[QTD_THREADS] = {0};

    int i;
    for (i = 0; i < QTD_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, printData, (void *)&data_arr[i]);
        pthread_join(threads[i], NULL);
    }

    return 0;
}
