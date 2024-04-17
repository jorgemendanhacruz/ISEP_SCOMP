#ifndef DATA_H
#define DATA_H

typedef struct{
    int arr[10];
    int terminated[10];
    int new_data;
}data;

#define SHM_NAME "/shmex05"

#endif