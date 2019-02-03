#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
// These values MUST match the unistd_32.h modifications:
#define __NR_cs3013_syscall1 377

long testCall1 ( void) {
return (long) syscall(__NR_cs3013_syscall1);
}


int main () {
printf("The return values of the system calls are:\n");
printf("\tcs3013_syscall1: %ld\n", testCall1());

if(fopen("virus", "r") == NULL || fopen("not_virus", "r") == NULL) {
	printf("FILES DO NOT EXIST");
	return -1;
}

char buffer1[200];
char buffer2[200];

FILE* virus;
FILE* not_virus;

virus = fopen("virus", "r");
fscanf(virus, "%s", buffer1);

fclose(virus);

not_virus = fopen("not_virus", "r");
fscanf(not_virus, "%s", buffer2);

fclose(not_virus);

return 0;
}
