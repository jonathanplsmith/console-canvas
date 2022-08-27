CC=gcc

all: main

main: main.c consoleCanvas.h
	$(CC) -o demo main.c

clean:
	rm demo