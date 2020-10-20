CC = gcc -std=gnu99
CFLAGS = -Wall -g 

all: ServerG ServerC 

ServerG: ServerG.c soc.h sharedCode.h
		$(CC) -o ServerG ServerG.c

ServerC: ServerC.c soc.h sharedCode.h
		$(CC) -o ServerC ServerC.c

clean:
	rm -f ServerG
	rm -f ServerC