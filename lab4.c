#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>

#define FILENAME "file.txt"
#define SEMAPHORE_NAME "/my_semaphore"
#define BUFFER_SIZE 1024

int main()
{
    sem_t *sem;
    int fd;
    char *map;
    int i, j;
    int counter = 0;

    // Создаем или открываем именованный семафор
    sem = sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED)
    {
        sem = sem_open(SEMAPHORE_NAME, 0);
    }

    // Открываем файл
    fd = open(FILENAME, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Определяем размер файла и изменяем его размер, если необходимо
    struct stat st;
    fstat(fd, &st);
    if (st.st_size < BUFFER_SIZE)
    {
        ftruncate(fd, BUFFER_SIZE);
    }

    // Отображаем файл в память
    map = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Записываем символы в файл и выводим их на экран
    while (1)
    {
        // Ожидаем сигнала от семафора
        sem_wait(sem);

        // Записываем символ '1' в файл
        map[counter++] = '1';

        // Выводим символ '1' на экран
        printf("1");

        // Если достигнут конец буфера, начинаем запись сначала
        if (counter == BUFFER_SIZE)
        {
            counter = 0;
        }

        // Отправляем сигнал другим процессам
        sem_post(sem);

        // Проверяем, была ли нажата клавиша 'q', и выходим из программы, если была
        fd_set fds;
        struct timeval tv;
        int retval;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        retval = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (retval == -1)
        {
            perror("select()");
            exit(EXIT_FAILURE);
        }
        else if (retval)
        {
            int c = getchar();
            if (c == 'q')
            {
                break;
            }
        }

        usleep(500000);
    }

    // Удаляем именованный семафор
    sem_unlink(SEMAPHORE_NAME);

    // Закрываем файл и освобождаем память
    munmap(map, BUFFER_SIZE);
    close(fd);

    return 0;
}
