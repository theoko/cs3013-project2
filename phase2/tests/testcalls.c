#include<sys/syscall.h>
#include<stdio.h>
#include<stdlib.h>
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
		while(1) {
			// Wasting CPU cycles...
		}
        } else {
		return (long) syscall(__NR_cs3013_syscall2, pid_num, proc);
	}

	return 0;
}

int main() {
	unsigned short *pid_num = malloc(sizeof(unsigned short));
	struct ancestry* proc = (struct ancestry*) malloc(sizeof(struct ancestry));

	printf("The return values of the system calls are:\n");
	printf("\tcs3013_syscall2: %ld\n", testCall2(pid_num, proc));
	
	for(int i=0; i<3; i++) {
		printf("Parent %d: %d\n", i, proc->ancestors[i]);

	}

}
