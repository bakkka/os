#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pid_t parent_pid = getpid();
    pid_t my_pid = getpid();

    printf("Program 1:\n");
    printf("My PID is %d\n", my_pid);
    printf("My parent's PID is %d\n", parent_pid);

    int i;
    for (i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
        sleep(1);
    }

    int exit_code = 42;
    printf("Program 1 is exiting with code %d\n", exit_code);
    exit(exit_code);
}
Программа 2:

c
Copy code
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
        execle("./program1", "program1", argv[1], argv[2], NULL, envp);
        perror("exec");
        exit(1);
    } else {
        printf("My child's PID is %d\n", child_pid);
        while (waitpid(child_pid, &status, WNOHANG) == 0) {
            printf("Waiting for Program 1 to finish...\n");
            sleep(1);
        }
        if (WIFEXITED(status)) {
            printf("Program 1 exited with code %d\n", WEXITSTATUS(status));
        }
    }

    printf("Program 2 is exiting.\n");
    return 0;
}
В программе 2 используется функция execle() для запуска программы 1 как дочернего процесса с передачей ему аргументов командной строки и пустого массива пе

network error


