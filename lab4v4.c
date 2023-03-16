#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define SEM_NAME "/my_sem"
#define FILE_NAME "shared_file.txt"
#define BUFFER_SIZE 1024

int main() {
    sem_t* sem;
    int fd;
    char buffer[BUFFER_SIZE];
    int i;

    sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        for (i = 0; i < 10; i++) {
            printf("1");
            fflush(stdout);
            buffer[i] = '1';
            usleep(10000);
        }
        write(fd, buffer, 10);
        if (sem_post(sem) == -1) {
            perror("sem_post");
            exit(1);
        }
    }

    close(fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);

    return 0;
}
Program 2:

c
Copy code
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define SEM_NAME "/my_sem"
#define FILE_NAME "shared_file.txt"
#define BUFFER_SIZE 1024

int main() {
    sem_t* sem;
    int fd;
    char buffer[BUFFER_SIZE];
    int i;

    sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    fd = open(FILE_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sem_wait(sem);
        read(fd, buffer, 10);
        for (i = 0; i < 10; i++) {
            printf("2");
            fflush(stdout);
            usleep(10000);
        }
        sem_post(sem);
        if (getchar() != EOF) {
            break;
        }
    }

    close(fd);
    sem_close(sem);

    return 0;
}
In this implementation, program 1 writes the character '1' to the terminal and to the file 'shared_file.txt' ten times, then releases the semaphore and waits for program 2 to write its characters. Program 2 reads from the

network error
Share Prompt


