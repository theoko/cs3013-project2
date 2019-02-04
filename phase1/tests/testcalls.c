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

char buffer1[200];
char buffer2[200];

FILE* virus;
FILE* not_virus;

virus = fopen("virus", "r");
if(virus == NULL) {
	printf("== Avast v0.1 ==\n");
	printf("We're clean!");
}

fscanf(virus, "%s", buffer1);

not_virus = fopen("not_virus", "r");
if(not_virus == NULL) {
	printf("== Avast 0.1 ==\n");
	printf("File is clean!");
}
fscanf(not_virus, "%s", buffer2);

fclose(virus);
fclose(not_virus);

return 0;
}
