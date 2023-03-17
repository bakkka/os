#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/select.h>

#define SEM_NAME "/mysemaphore"
#define FILE_NAME "text.txt"

int main() {
    sem_t* sem;
    int fd;
    char symbol = '1';
    int flag_exit = 0;
    int i;
    
    // Create or open the named semaphore
    sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    
    // Create or open the file
    fd = open(FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    
    // Write 10 characters to the file and alternate with program 2
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
        // Wait for a key press before continuing
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        select(STDIN_FILENO + 1, &rfds, NULL, NULL, NULL);
        sleep(1);
    }
        
    
    // Close and unlink the semaphore
    sem_close(sem);
    sem_unlink(SEM_NAME);
    
    // Close the file
    close(fd);
    
    return 0;
}
