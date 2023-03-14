#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("Program 2 started\n");
    printf("Parent process ID: %d\n", getpid());
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process ID: %d\n", getpid());
        char *args[argc + 1];
        for (int i = 0; i < argc; i++) {
            args[i] = argv[i];
        }
        args[argc] = NULL;
        char *envp[] = {NULL};
        execle("./program1", "./program1", args, envp);
        printf("Error: execle failed\n");
        exit(1);
    } else if (pid > 0) {
        int status;
        while (waitpid(pid, &status, WNOHANG) == 0) {
            printf("Waiting for child process to finish...\n");
            usleep(500000); // 0.5 second
        }
        if (WIFEXITED(status)) {
            printf("Child process exited with code %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        }
    } else {
        printf("Error: fork failed\n");
        exit(1);
    }
    printf("Program 2 finished\n");
    return 0;
}
