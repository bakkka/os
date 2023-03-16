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
    char buffer[2];

    fd_set readset; // Создаем набор дескрипторов для функции select
    FD_ZERO(&readset); // Обнуляем набор

    FD_SET(fileno(stdin), &readset); // Добавляем дескриптор стандартного потока ввода

    while(1)
    {
        for(int i=0; i<10; i++)
        {
            fprintf(outFile, "%c", myChar); // Пишем символ в файл
            printf("%c", myChar); // Выводим символ на экран
            fflush(stdout); // Сбрасываем буфер

            usleep(100000); // Задержка 100 миллисекунд
        }

        fprintf(outFile, "\n");
        printf("\n");

        // Освобождаем семафор, чтобы программа "2" могла его захватить и записать символ "2" 10 раз
        sem_post(mySemaphore);

        // Ждем пока программа "2" не завершит свою итерацию и не отправит сигнал семафору
        sem_wait(mySemaphore);

        // Меняем символ на "2", чтобы программа "2" смогла записать его в следующей итерации
        myChar = '2';

        // Ждем пока произойдет изменение во входных данных на стандартном потоке ввода или произойдет изменение на другом дескрипторе в течение 500 миллисекунд
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        int result = select(fileno(stdin) + 1, &readset, NULL, NULL, &timeout);

        if(result == -1)
        {
            printf("Select Error!\n");
            return -1;
        }
        else if(result)
        {
            fgets(buffer, 2, stdin);

            if(buffer[0] == 'q') // Проверяем, на какой клавише нажали, если на "q", то завершаем работу программы
            {
                break;
            }
        }
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
    char buffer[2];

    fd_set readset; // Создаем набор дескрипторов для функции select
    FD_ZERO(&readset); // Обнуляем набор

    FD_SET(fileno(stdin), &readset); // Добавляем дескриптор стандартного потока ввода

    while(1)
    {
        for(int i=0; i<10; i++)
        {
            fprintf(outFile, "%c", myChar); // Пишем символ в файл
            printf("%c", myChar); // Выводим символ на экран
            fflush(stdout); // Сбрасываем буфер

            usleep(100000); // Задержка 100 миллисекунд
        }

        fprintf(outFile, "\n");
        printf("\n");

        // Освобождаем семафор, чтобы программа "1" могла его захватить и записать символ "1" 10 раз
        sem_post(mySemaphore);

        // Ждем пока программа "1" не завершит свою итерацию и не отправит сигнал семафору
        sem_wait(mySemaphore);

        // Меняем символ на "1", чтобы программа "1" смогла записать его в следующей итерации
        myChar = '1';

        // Ждем пока произойдет изменение во входных данных на стандартном потоке ввода или произойдет изменение на другом дескрипторе в течение 500 миллисекунд
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        int result = select(fileno(stdin) + 1, &readset, NULL, NULL, &timeout);

        if(result == -1)
        {
            printf("Select Error!\n");
            return -1;
        }
        else if(result)
        {
            fgets(buffer, 2, stdin);

            if(buffer[0] == 'q') // Проверяем, на какой клавише нажали, если на "q", то завершаем работу программы
            {
                break;
            }
        }
    }

    fclose(outFile);

    if(sem_unlink("/mySemaphore") == -1)
    {
        printf("Semaphore Deletion Failed !\n");
        return -1;
    }

    return 0;
}
