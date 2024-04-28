#ifndef DATA_H
#define DATA_H

typedef struct{
    char phrase[100]; 
    int *childs;
    int new_data;
}data;

#define SHM_NAME "/shmex06"

#endif