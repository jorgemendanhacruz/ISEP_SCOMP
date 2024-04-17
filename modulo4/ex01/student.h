#ifndef STUDENT_H
#define STUDENT_H

#define SHM_NAME "shmex01"

typedef struct
{
    int number;
    char name[30];
    char address[50];
}student;

#endif