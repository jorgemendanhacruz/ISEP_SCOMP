#ifndef DATA_H
#define DATA_H

typedef struct{
    char phrase[100]; 
    int current_char;
    int new_data;
    int childs[];
}data;

#define SHM_NAME "/shmex06"

#endif