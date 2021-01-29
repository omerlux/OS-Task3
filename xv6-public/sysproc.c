#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//------ 06.01.21 ------
//system call - replace the old signal handler to new signal handler and return the old one.
int
sys_sigset(void)
{
    sig_handler new_sigHandler;
    if(argptr(0, (void*)&new_sigHandler,sizeof(sig_handler)) < 0) //todo:check it!!!!
        return -1;
    sig_handler old_sigHandler = myproc()->sigHandler;
    myproc()->sigHandler = new_sigHandler;
    cprintf("(sysproc.c) sys_sigset: for process %d.\n",myproc()->pid);

    return (int)(old_sigHandler);
}

//system call - send signal "value" from current processs to dest_pid process.
int
sys_sigsend(void)
{
    int dest_pid;
    int value;
    if(argint(0, &dest_pid) < 0)
        return -1;
    if(argint(1, &value) < 0)
        return -1;
    cprintf("(sysproc.c) sys_sigsend: from process %d - to pid %d with value %d.\n",myproc()->pid, dest_pid, value);
    return sigsend(dest_pid,value);
}

//------ 09.01.21 ------
//system call - sigret
void
sys_sigret(void)
{
	sigret();
	return;
}
//--------------------