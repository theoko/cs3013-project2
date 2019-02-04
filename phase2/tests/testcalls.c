#include<sys/syscall.h>
#include<stdio.h>
#include<unistd.h>
#include "test.h"

#define __NR_cs3013_syscall2 378

long testCall2(void) {
	// Test curr->parent
        int pid = fork();

	if(pid < 0) {
		printf("FAILED!");
	}

        // Child
        if(pid == 0) {
		sleep(5000);
        } else {
		
        }

	return (long) syscall(__NR_cs3013_syscall2);
}

int main() {
	
}
