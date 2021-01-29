#include "mmu.h"
#include "syscall.h"
#include "traps.h"


  # vectors.S sends all traps here.
.globl alltraps
alltraps:
  # Build trap frame.
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs
  pushal
  
  # Set up data segments.
  movw $(SEG_KDATA<<3), %ax
  movw %ax, %ds
  movw %ax, %es

  # Call trap(tf), where tf=%esp
  pushl %esp
  call trap
  addl $4, %esp

  # Return falls through to trapret...
.globl trapret
trapret:
  # ------ 06.01.21 ------
  pushl %esp
  call signals_check		# check for pending signals, if there are, handle them if you available
  addl $4, %esp
  # ------------------------
  popal
  popl %gs
  popl %fs
  popl %es
  popl %ds
  addl $0x8, %esp  # trapno and errcode
  iret


# ------ 10.01.21 ------
.global implicit_sigret_start
.global implicit_sigret_end
implicit_sigret_start:					# inserting a call to the sigret sys_call
	movl $SYS_sigret, %eax		
	int $T_SYSCALL						# User code makes a system call with INT T_SYSCALL.
implicit_sigret_end:
# --------------------------
