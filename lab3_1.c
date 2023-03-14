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
