CFLAGS = -Wall -g

all: testcalls

testcalls: testcalls.o
	gcc $(CFLAGS) testcalls.o -o testcalls

testcalls.o: testcalls.c
	gcc $(CFLAGS) -c testcalls.c

clean:
	rm -f *.o testcalls
