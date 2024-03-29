#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/list.h>

typedef struct task_struct task_struct;

unsigned long **sys_call_table;

asmlinkage long (*ref_sys_cs3013_syscall2)(void);

typedef struct ancestry {
	pid_t ancestors[10];
	pid_t siblings[100];
	pid_t children[100];
} ancestry;

void print_ancestors(task_struct *parent, pid_t *ancesPtr) {
	
	// PID of the current ancestor
	pid_t anc_pid = parent->pid;

	// Add the PID to the array
	*ancesPtr++ = anc_pid;

	// Print parent to log
	printk(KERN_INFO "Parent: %d\n", anc_pid);

	// Check if PID is valid
	if(anc_pid == 0 || anc_pid == 1) {
		return;
	}

	// Find parent of parent
	parent = parent->parent;

	// Recursively find ancestors
	print_ancestors(parent, ancesPtr);

}

asmlinkage long new_sys_cs3013_syscall2(unsigned short *target_pid, struct ancestry *response) {
	
	task_struct* curr;
	task_struct* parent; 
        task_struct* proc;

	pid_t temp;

	pid_t* parentPtr;
        pid_t* siblingPtr;
        pid_t* childPtr;

	unsigned short arg_pid;
	ancestry arg_anc;
	ancestry* arg_ancestry;
	arg_ancestry = &arg_anc;

	if(copy_from_user(&arg_pid, target_pid, sizeof(unsigned short)))
		return EFAULT;

	if(copy_from_user(arg_ancestry, response, sizeof(struct ancestry)))
                return EFAULT;


	// Pointer to  ancestors
	parentPtr = arg_ancestry->ancestors;
        
	// Pointer to siblings
	siblingPtr = arg_ancestry->siblings;
        
	// Pointer to children
	childPtr = arg_ancestry->children;	

	// Find task_struct of process using its PID
	curr = pid_task(find_vpid(arg_pid), PIDTYPE_PID);

	// Print this process's ID
	printk(KERN_INFO "Process ID: %d\n", curr->pid);

	if(curr->pid != 1) {
		
		parent = curr->parent;
		
		print_ancestors(parent, parentPtr);
	
	}

	// Iterate over doubly linked list to find children
        list_for_each_entry(proc, &(curr->children), sibling) {

                temp = proc->pid;

                // Store PID
                *childPtr = temp;

                // Increment pointer
                childPtr++;

                printk(KERN_INFO "%d's child: %d!\n", curr->pid, temp);

        }

	// Iterate over doubly linked list to find siblings
	list_for_each_entry(proc, &(curr->sibling), sibling) {

                temp = proc->pid;

                if (temp == 0) 
                        return temp;

		// Store PID
                *siblingPtr = temp;

		// Increment pointer
                siblingPtr++;

		printk(KERN_INFO "%d's sibling: %d!\n", curr->pid, temp);
								
        }

	// Copy struct passed as argument from user
	if(copy_to_user(response, arg_ancestry, sizeof(struct ancestry)))
		return EFAULT;


	return 0;

}

static unsigned long **find_sys_call_table(void) {
  unsigned long int offset = PAGE_OFFSET;
  unsigned long **sct;
  
  while (offset < ULLONG_MAX) {
    sct = (unsigned long **)offset;

    if (sct[__NR_close] == (unsigned long *) sys_close) {
      printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX",
	     (unsigned long) sct);
      return sct;
    }
    
    offset += sizeof(void *);
   }
  
    return NULL;
}

static void disable_page_protection(void) {
  /*
    Control Register 0 (cr0) governs how the CPU operates.

    Bit #16, if set, prevents the CPU from writing to memory marked as
    read only. Well, our system call table meets that description.
    But, we can simply turn off this bit in cr0 to allow us to make
    changes. We read in the current value of the register (32 or 64
    bits wide), and AND that with a value where all bits are 0 except
    the 16th bit (using a negation operation), causing the write_cr0
    value to have the 16th bit cleared (with all other bits staying
    the same. We will thus be able to write to the protected memory.

    It's good to be the kernel!
  */
  write_cr0 (read_cr0 () & (~ 0x10000));
}

static void enable_page_protection(void) {
  /*
   See the above description for cr0. Here, we use an OR to set the 
   16th bit to re-enable write protection on the CPU.
  */
  write_cr0 (read_cr0 () | 0x10000);
}

static int __init interceptor_start(void) {
  /* Find the system call table */
  if(!(sys_call_table = find_sys_call_table())) {
    /* Well, that didn't work. 
       Cancel the module loading step. */
    return -1;
  }
  
  /* Store a copy of all the existing functions */
  ref_sys_cs3013_syscall2 = (void *)sys_call_table[__NR_cs3013_syscall2];

  /* Replace the existing system calls */
  disable_page_protection();

  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)new_sys_cs3013_syscall2;

  enable_page_protection();
  
  /* And indicate the load was successful */
  printk(KERN_INFO "Loaded interceptor!");

  return 0;
}

static void __exit interceptor_end(void) {
  /* If we don't know what the syscall table is, don't bother. */
  if(!sys_call_table)
    return;
  
  /* Revert all system calls to what they were before we began. */
  disable_page_protection();
  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)ref_sys_cs3013_syscall2;

  enable_page_protection();

  printk(KERN_INFO "Unloaded interceptor!");
}

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);
