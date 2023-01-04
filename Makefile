CC=gcc

all: main library

main: main.c library
	$(CC) -Wall -Wextra -O2 -o demo main.c consoleCanvas.o

library: consoleCanvas.c consoleCanvas.h
	$(CC) -Wall -Wextra -O2 -c consoleCanvas.c -o consoleCanvas.o

clean:
	rm demo consoleCanvas.o