#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include "student.h"

int main(int argc, char* argv[])
{
    const int DATA_SIZE = sizeof(student);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, DATA_SIZE);
    student *students_1 = (student *)mmap(NULL, DATA_SIZE,
                                          PROT_READ | PROT_WRITE, 
                                          MAP_SHARED, fd, 0);

    printf("Student number: %d\n", students_1->number);
    

    printf("Student name: %s\n", students_1->name);
    

    printf("Student address: %s\n", students_1->address);



    
    

    return 0;
}
