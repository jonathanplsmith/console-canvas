#include "consoleCanvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define DELAY 25000

int main(int argc, char* args[]) {

    canvas cons = newCanvas(21, 24);

    placeTextHor(&cons, "You can write text to the console", 2, 5);
    refreshConsole(&cons);
    sleep(1);
    placeTextVert(&cons, "Vertically too!", 4, 5);
    refreshConsole(&cons);
    sleep(3);
    
    clearCanvas(&cons);
    placeTextHor(&cons, "You can draw characters to the console", 2, 5);
    refreshConsole(&cons);
    sleep(1);
    for (int i=0; i<cons.height; i++) {
        inputChar(&cons, '*', i, 0);
        refreshConsole(&cons);
        usleep(DELAY);
    }
    for (int i=0; i<cons.width; i++) {
        inputChar(&cons, '*', cons.height-1, i);
        refreshConsole(&cons);
        usleep(DELAY); 
    }
    for (int i=cons.height-1; i>=0; i--) {
        inputChar(&cons, '*', i, cons.width-1);
        refreshConsole(&cons);
        usleep(DELAY);
    }
    for (int i=cons.width-1; i>=0; i--) {
        inputChar(&cons, '*', 0, i);
        refreshConsole(&cons);
        usleep(DELAY);
    }
    clearCanvas(&cons);
    placeTextHor(&cons, "Or you could draw some lines", 2, 5);
    refreshConsole(&cons);
    sleep(1);

    clearCanvas(&cons);
    drawLine(&cons, '#', 0, 11, 9, 0);
    refreshConsole(&cons);
    usleep(10 * DELAY);
    drawLine(&cons, '#', 9, 0, 20, 9);
    refreshConsole(&cons);
    usleep(10 * DELAY);
    drawLine(&cons, '#', 20, 9, 11, 20);
    refreshConsole(&cons);
    usleep(10 * DELAY);
    drawLine(&cons, '#', 0, 11, 11, 20);
    refreshConsole(&cons);
    usleep(10 * DELAY);
    drawLine(&cons, 'X', 0, 0, 20, 20);
    refreshConsole(&cons);
    usleep(10 * DELAY);
    drawLine(&cons, 'X', 20, 0, 0, 20);
    refreshConsole(&cons);
    sleep(1);

    clearCanvas(&cons);
    placeTextHor(&cons, "Or how about a sprite?", 2, 5);
    refreshConsole(&cons);
    sleep(1);

    #define ROW 4
    #define COL 5
    char smiley [ROW][COL] = {
        {' ', '|', ' ', '|', ' '},
        {' ', '|', ' ', '|', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {'\\', '_', '_', '_', '/'}
    };
    char** sprite = malloc(sizeof(char*) * ROW);
    for (int i=0; i < ROW; i++) {
        sprite[i] = malloc(sizeof(char) * COL);
    }
    for (int i=0; i < ROW; i++) {
        for (int j=0; j<COL; j++) {
            sprite[i][j] = smiley[i][j];
        }
    }
    clearCanvas(&cons);
    drawSprite(&cons, sprite, ROW, COL, 10, 10);
    refreshConsole(&cons);
    usleep(25 * DELAY);
    clearCanvas(&cons);
    drawSprite(&cons, sprite, ROW, COL, 2, 4);
    refreshConsole(&cons);
    usleep(25 * DELAY);
    clearCanvas(&cons);
    drawSprite(&cons, sprite, ROW, COL, 15, 9);
    refreshConsole(&cons);

    for (int i=0; i < ROW; i++) {
        free(sprite[i]);
    }
    free(sprite);

    deleteCanvas(&cons);
    
    return 0;
}