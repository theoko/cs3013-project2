#include<sys/syscall.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include "test.h"

#define __NR_cs3013_syscall2 378


long testCall2(unsigned short *pid_num, struct ancestry *proc) {
	return (long) syscall(__NR_cs3013_syscall2, pid_num, proc);
}

int main(int argc, char *argv[]) {
	
	// Check argument count
	if(argc < 2) {
		printf("Please enter a PID as the first argument");
		return -1;
	}

	unsigned short pid = atoi(argv[1]);
	
	struct ancestry* proc = (struct ancestry*) malloc(sizeof(struct ancestry));
	
	printf("The return values of the system calls are:\n");
	printf("\tcs3013_syscall2: %ld\n", testCall2(&pid, &proc));
}
