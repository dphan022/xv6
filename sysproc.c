#include "types.h"
#include "x86.h"
#include "defs.h"
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
  int status;
  argint(0,&status);
  exit(status);
  return 0;  // not reached
}

// Lab 1 - Part b
int
sys_wait(void)
{
  int* status;          
  char* p = 0;          // p is the address of pointer in the syscall's arg
  argptr(0, &p, 4);     // 4 bytes is the size of an int
  if(p == 0)
    return wait(0);
  status = (int*)p;
  return wait(status);
}

// Lab 1 - Part c
int
sys_waitpid(void)
{
  int *status, pid, options;
  // 4 bytes is the size of an int 
  if( (argptr(0, (char**) &pid,     4) < 0) ||
      (argptr(1, (char**) &status,  4) < 0) ||
      (argptr(2, (char**) &options, 4) < 0)   ){
      return -1;
  }
  return waitpid(pid, status, options);
}
//Fetch the int priority  at addr from the current process.
//if it is < 0, then return -1, no priority to be changed.
int
sys_change_priority(void){
    int priority;
    if(argint(0, &priority) < 0)
        return -1;
    return change_priority(priority);
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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
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
