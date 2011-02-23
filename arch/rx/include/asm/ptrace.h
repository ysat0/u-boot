#ifndef __ASM_RX_PTRACE_H__
#define __ASM_RX_PTRACE_H__

#ifndef __ASSEMBLY__

/* this struct defines the way the registers are stored on the
   stack during a system call. */

struct pt_regs {
	unsigned long r[16];
	unsigned long usp;
	unsigned long vec;
	unsigned long pc;
	unsigned long psw;
};

/* Find the stack offset for a register, relative to thread.esp0. */
#define PT_REG(reg)	((long)&((struct pt_regs *)0)->reg)

#define user_mode(regs) (!((regs)->psw & 0x00100000))
#define instruction_pointer(regs) ((regs)->pc)
#define profile_pc(regs) instruction_pointer(regs)
extern void show_regs(struct pt_regs *);
#endif /* __ASSEMBLY__ */
#endif /* __ASM_RX_PTRACE_H__ */
