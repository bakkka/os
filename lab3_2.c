#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t parent_pid = getpid();
    pid_t child_pid;
    int status;

    printf("Program 2:\n");
    printf("My PID is %d\n", parent_pid);

    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(1);
    }

    if (child_pid == 0) {
        char *envp[] = { NULL }; // пустой массив переменных окружения
        printf("Running Program 1 as a child process...\n");
        execle("./lab3_1", "lab3_1", argv[1], argv[2], NULL, envp);
        perror("exec");
        exit(1);
    } else {
        printf("My child's PID is %d\n", child_pid);
        while (waitpid(child_pid, &status, WNOHANG) == 0) {
            printf("Waiting for Program 1 to finish...\n");
            usleep(500000); // 0.5 second
        }
        if (WIFEXITED(status)) {
            printf("Program 1 exited with code %d\n", WEXITSTATUS(status));
        }
    }

    printf("Program 2 is exiting.\n");
    return 0;
}
