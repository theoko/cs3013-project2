CFLAGS = -Wall -g

all: testcalls procAncestry

testcalls: testcalls.o
	gcc $(CFLAGS) testcalls.o -o testcalls

testcalls.o: testcalls.c
	gcc $(CFLAGS) -c testcalls.c

procAncestry: procAncestry.o
	gcc $(CFLAGS) procAncestry.o -o procAncestry

procAncestry.o: procAncestry.c
	gcc $(CFLAGS) -c procAncestry.c

clean:
	rm -f *.o testcalls procAncestry
