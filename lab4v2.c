#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define FILENAME "myFile.txt"

int main()
{
    sem_t *mySemaphore;

    mySemaphore = sem_open("/mySemaphore", O_CREAT, S_IWUSR | S_IRUSR, 1); // Создаем семафор с именем "/mySemaphore"

    if(mySemaphore == SEM_FAILED)
    {
        printf("Semaphore Opening Failed !\n");
        return -1;
    }

    FILE *outFile;
    outFile = fopen(FILENAME, "a");

    if(outFile == NULL)
    {
        printf("File Opening Failed!\n");
        return -1;
    }

    char myChar = '1';
    int count = 0;
    
    while(1){
        while(count < 5) // Записываем символ 5 раз
    {
        sem_wait(mySemaphore); // Захватываем семафор

        fprintf(outFile, "%c\n", myChar); // Пишем символ в файл
        printf("%c\n", myChar); // Выводим символ на экран

        count++; // Увеличиваем счетчик записанных символов

        sem_post(mySemaphore); // Освобождаем семафор

        usleep(500000); // Ждем полсекунды
    }

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
    

    fclose(outFile);

    if(sem_unlink("/mySemaphore") == -1)
    {
        printf("Semaphore Deletion Failed !\n");
        return -1;
    }

    return 0;
}
Программа 2 (пишет в файл символ "2"):

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define FILENAME "myFile.txt"

int main()
{
    sem_t *mySemaphore;

    mySemaphore = sem_open("/mySemaphore", O_CREAT, S_IWUSR | S_IRUSR, 1); // Создаем семафор с именем "/mySemaphore"

    if(mySemaphore == SEM_FAILED)
    {
        printf("Semaphore Opening Failed !\n");
        return -1;
    }

    FILE *outFile;
    outFile = fopen(FILENAME, "a");

    if(outFile == NULL)
    {
        printf("File Opening Failed!\n");
        return -1;
    }

    char myChar = '2';

    while(1)
    {
        sem_wait(mySemaphore); // Захватываем семафор

        fprintf(outFile, "%c\n", myChar); // Пишем символ в файл
        printf("%c\n", myChar); // Выводим символ на экран

        if(myChar == '1')
        {
            myChar = '2'; // Меняем символ на "2"
        }
        else
        {
            myChar = '1'; // Меняем символ на "1"
        }

        sem_post(mySemaphore); // Освобождаем семафор

        usleep(500000); // Ждем полсекунды
    }

    fclose(outFile);

    if(sem_unlink("/mySemaphore") == -1)
    {
        printf("Semaphore Deletion Failed !\n");
        return -1;
    }

    return 0;
}
