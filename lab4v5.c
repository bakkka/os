#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <poll.h>

#define SEM_NAME "/my_semapho" 
#define FILE_NAME "file"
#define BUFFER_SIZE 1024

int main() {

    sem_t* sem;
    int fd;
    char symbol = '1';
    int flag_exit = 0;
    int i;

    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    sem = sem_open(SEM_NAME, O_CREAT, 0777, 1);
    if (sem == SEM_FAILED) {
        perror("Ошибка при создании семафора");
        exit(EXIT_FAILURE);
    } else {
        printf("Семафор создан\n");
         usleep(500000);
    }

    fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("Ошибка при создании файла");
        exit(EXIT_FAILURE);
    } else {
        printf("Файл открыт\n");
         usleep(500000);
    }

    while (!flag_exit) {

        if (sem_wait(sem) == -1) {
            perror("Ошибка при захвате семафора");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < 10; i++) {
            write(fd, &symbol, 1);
            putchar(symbol);
            fflush(stdout);
            sleep(1);
        }

        if (sem_post(sem) == -1) {
            perror("Ошибка при освобождении семафора");
            exit(EXIT_FAILURE);
        }
   
        if (poll(fds, 1, 1000) > 0) {
            flag_exit = 1;
            printf("\nКлавиша нажата\n");
            break;
        }
        sleep(1);
    }

    if (close(fd) == -1) {
        perror("Ошибка при закрытии файла");
        exit(EXIT_FAILURE);
    } else {
        printf("Файл закрыт\n");
         usleep(500000);
    }
    if (sem_close(sem) == -1) {
        perror("Ошибка при закрытии семафора");
        exit(EXIT_FAILURE);
    } else {
        printf("Семафор закрыт\n");
         usleep(500000);
    }
    
    if (sem_unlink(SEM_NAME) == -1) {
        perror("Ошибка при удалении семафора или он уже удален");
        exit(EXIT_FAILURE);
    } else {
        printf("Семафор удален\n");
         usleep(500000);
    }

    return 0;
}
