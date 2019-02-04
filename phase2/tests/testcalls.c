#include<sys/syscall.h>
#include<stdio.h>
#include<unistd.h>
#include "test.h"

#define __NR_cs3013_syscall2 378

long testCall2(unsigned short *pid_num, struct ancestry *proc) {
	// Test curr->parent
        int pid = fork();

	if(pid < 0) {
		printf("FAILED!");
	}

        // Child
        if(pid == 0) {
		sleep(5000);
        } else {

		return (long) syscall(__NR_cs3013_syscall2, pid_num, proc);
	}
}

int main() {
	unsigned short *pid_num = malloc(sizeof(unsigned short));
	struct ancestry* proc = (struct ancestry*) malloc(sizeof(struct ancestry));

	printf("The return values of the system calls are:\n");
	printf("\tcs3013_syscall2: %ld\n", testCall2(&pid, proc));
	
	for(int i=0; i<3; i++) {
		printf("Parent %d: %d\n", proc->ancestors[i]);
	}

}
