#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

// function declaret from vm.c ---------------------------------------------------------------
// Return the address of the PTE in page table pgdir
// that corresponds to virtual address va.  If alloc!=0,
// create any required page table pages.
extern pte_t *walkpgdir(pde_t *pgdir, const void *va, int alloc);
// -------------------------------------------------------------------------------------------

// numvp() system call definition ------------------------------------------------------------
int sys_numvp(void)
{
  struct proc *current_process = myproc(); // get current process

  // get the number of virtual pages
  int virtual_pages = PGROUNDUP(current_process->sz) / PGSIZE;

  return virtual_pages + 1; // +1 to include the stack guard page
}
// -------------------------------------------------------------------------------------------

// numpp() system call definition ------------------------------------------------------------
int sys_numpp(void)
{
  struct proc *current_process = myproc(); // get current process

  int physical_pages = 0;

  pte_t *pte;

  for (int i = 0; i < current_process->sz; i += PGSIZE)
  {
    // i represents the virual page address for each virtual page page
    // check if the physical page corresponding to i is present
    pte = walkpgdir(current_process->pgdir, (void *)i, 0);
    if (pte && (*pte & PTE_P))
    {
      physical_pages++;
    }

    // check if the physical page corresponding to the stack guard page is present
    pte = walkpgdir(current_process->pgdir, (void *)PGROUNDUP(current_process->sz), 0);
    if (pte && (*pte & PTE_P))
    {
      physical_pages++;
    }
  }

  return physical_pages;
}
// -------------------------------------------------------------------------------------------

// mmap() system call definition ------------------------------------------------------------
int sys_mmap(void)
{
  int size;

  // check if the size given is valid
  if (argint(0, &size) < 0 || size <= 0 || size % PGSIZE != 0)
  {
    return -1;
  }

  struct proc *current_process = myproc();       // get current process
  uint address = PGROUNDUP(current_process->sz); // get the address of the next page after the last page of the process

  // increase the virtual arddress space
  current_process->sz += size;
  // physical page is allocated in trap.c while handling page fault

  return address;
}
// -------------------------------------------------------------------------------------------

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
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
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
