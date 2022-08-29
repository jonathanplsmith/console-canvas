CC=gcc

all: main library

main: main.c library
	$(CC) -o demo.out main.c consoleCanvas.o

library: consoleCanvas.c consoleCanvas.h
	$(CC) -c consoleCanvas.c -o consoleCanvas.o

clean:
	rm demo.out consoleCanvas.o