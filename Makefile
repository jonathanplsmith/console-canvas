CC=gcc

all: main library

main: main.c library
	$(CC) -Wall -Wextra -o demo.out main.c consoleCanvas.o

library: consoleCanvas.c consoleCanvas.h
	$(CC) -Wall -Wextra -c consoleCanvas.c -o consoleCanvas.o

clean:
	rm demo.out consoleCanvas.o