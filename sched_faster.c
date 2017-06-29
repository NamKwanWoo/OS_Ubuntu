#include <linux/syscalls.h>
#include <linux/kernel.h>

#include <linux/module.h>
#include <linux/init.h>

int fast_PID;

asmlinkage void sys_sched_faster(int pid)
{
	//printk("\n");
	//printk("sched_faster.c file open %d\n", pid);
	fast_PID = pid;
	//printk("fast_PID = %d, extern slow_PID = %d\n", fast_PID, slow_PID);
}

EXPORT_SYMBOL(fast_PID);
