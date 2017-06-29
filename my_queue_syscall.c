// Location:  /linux-4.4/kernel

#include <linux/syscalls.h>
#include <linux/kernel.h>

#define MAX     5

int queue_array[MAX];
int rear = -1;
int front = -1;

// To Enqueue an integer. 326
asmlinkage void sys_namnamnam_enqueue(int n)
{
    int add_item, i;
    printk("New System Call Saying %d\n", n);
    printk("Enqueue: %d\n", n);

    // This queue is circular queue,  So  when it is finished, make it start position
    if (front == MAX)
    {
        front = -1;
        rear = -1;
    }

     // This means that queue is full
    if (rear == MAX - 1)
        printk("[Error] Queue Overflow \n");
    else
    {
        if (front == -1)
            front = 0;
        
        // For loop can find duplicate value by using linear search
        for (i = front; i <= rear; i++)
        {
            if (queue_array[i] == n)
            {
                printk("[Error] %d is already in Queue! \n", n);
                return;
            }
        }
        add_item = n;
        rear = rear + 1;    //index ++
        queue_array[rear] = add_item;
    }
}

// To Dequeue an integer. 327
asmlinkage int sys_namnamnam_dequeue(void)
{
    // -999 value is a error number
    // front == -1 is a initial condition
    if (front == -1 || front > rear)
    {
        printk("[Error] Queue Underflow, -999 means error \n");
        front = -1;
        rear = -1;
        return -999;
    }
    else
    {
        printk("Dequeue: %d\n", queue_array[front]);

        front = front + 1;
        return queue_array[front - 1];
    }
}


// To print Queue. 328
asmlinkage void sys_namnamnam_print(void)
{
    int i;
    // front == -1 is initial condition
    if (front == -1)
        printk("[Error] Queue is empty \n");
    else
    {
        printk("Queue:\t");
        for (i = front; i <= rear; i++)
            printk("%d ", queue_array[i]);
        printk("\n");
    }
}

