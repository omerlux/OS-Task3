//------ 06.01.21 ------

// defines an element of the concurrent struct
struct cstackframe{
    int sender_pid;
    int recipient_pid;
    int value;
    int used;
    struct cstackframe *next;
};

//defines a concurrent stack
struct cstack{
    struct cstackframe frames[10];
    struct cstackframe *head;
};

// adds a  new frame tot the cstack which is initialized with values send_pid,
// recipient_pid and value,
// then returns 1 on success and 0 if the stack is full
int push (struct cstack *cstack, int sender_pid, int recipient_pid, int value);

// removes and returns an element from the head of given cstack. if the stack is empty, then return 0
struct cstackframe *pop(struct cstack *cstack);

//------------end--------------

// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
  
  //------ 06.01.21 ------
  sig_handler sigHandler;
  struct cstack pending_signals;
   //------ 09.01.21 ------
   struct trapframe *tf_backup;	// backup for trapframe
   int handling_signal;				// handling signals flag
  //--------------------

};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap









