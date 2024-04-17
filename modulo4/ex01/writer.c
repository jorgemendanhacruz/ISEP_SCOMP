#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include "student.h"


int main()
{
    const int DATA_SIZE = sizeof(student);
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    if (ftruncate(fd, DATA_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    student *students_1 = (student *)mmap(NULL, DATA_SIZE,
                                          PROT_READ | PROT_WRITE, 
                                          MAP_SHARED, fd, 0);

    if (students_1 == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("Student number: ");
    scanf("%d", &students_1->number);
    // Clear the input buffer
    while (getchar() != '\n');

    printf("Student name: ");
    fgets(students_1->name, sizeof(students_1->name), stdin);

    printf("Student address: ");
    fgets(students_1->address, sizeof(students_1->address), stdin);


    

    return 0;
}
