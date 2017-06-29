#include <unistd.h>
#include <stdio.h>

#define ENQUEUE		326
#define DEQUEUE		327
#define PRINT		328

int main()
{
    int i = 0;
    int number;

    printf("---------1st Queue Test----------\n");

    for (i = 0; i <= 10; i++)
    {
        if (i < 5)
        {
            syscall(ENQUEUE, i*10);
            printf("Enqueue %d\n", i * 10);
        }
        else if (i == 5)
        {
            syscall(ENQUEUE, (i - 1) * 10);
            printf("Enqueue %d\n", (i - 1) * 10);
        }
        else
        {
            number = syscall(DEQUEUE);
            printf("Dequeue %d\n", number);
        }
        syscall(PRINT);
    }

    printf("---------2nd Queue Test----------\n");

    for (i = 0; i <= 10; i++)
    {
        if (i < 6)
        {
            syscall(ENQUEUE, i*10);
            printf("Enqueue %d\n", i * 10);
        }
        else if (i == 5)
        {
            syscall(ENQUEUE, 80);
            printf("Enqueue %d\n", 50);
        }
        else
        {
            number = syscall(DEQUEUE);
            printf("Dequeue %d\n", number);
        }
        syscall(PRINT);
    }


    printf("---------3rd Queue Test---------\n");

    for (i = 0; i <= 10; i++)
    {
        if (i < 6)
        {
            number = syscall(DEQUEUE);
            printf("Dequeue %d\n", number);
        }
        else
        {
            syscall(ENQUEUE, i*10);
            printf("Enqueue %d\n", i * 10);
        }
        syscall(PRINT);
    }

    return 0;
}
