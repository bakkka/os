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
    sem_t *sem;
    int fd;
    char c = '1';
    
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
    for (int i = 0; i < 10; i++) {
        // Wait for the semaphore to be available
        sem_wait(sem);
        
        // Write the character to the file
        write(fd, &c, 1);
        
        // Write the character to the terminal
        putchar(c);
        fflush(stdout);
        
        // Release the semaphore
        sem_post(sem);
        
        // Wait for a key press before continuing
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        select(STDIN_FILENO + 1, &rfds, NULL, NULL, NULL);
    }
    
    // Close and unlink the semaphore
    sem_close(sem);
    sem_unlink(SEM_NAME);
    
    // Close the file
    close(fd);
    
    return 0;
}
