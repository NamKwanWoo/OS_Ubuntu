#include <linux/syscalls.h>
#include <linux/kernel.h>

#include <linux/module.h>
#include <linux/init.h>

int slow_PID;

asmlinkage void sys_sched_slower(int pid)
{
	//printk("scehd_slower file open %d\n", pid);
	slow_PID = pid;
}

EXPORT_SYMBOL(slow_PID);
