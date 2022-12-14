#include "consoleCanvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DELAY 25000

int main(void) {

    canvas_t *cons = newCanvas(21, 24);

    setFg(cons, GREEN);
    setBg(cons, BLACK);
    placeTextHor(cons, "You can write colourful text to the console", 2, 3);
    refreshConsole(cons);
    sleep(1);
    setFg(cons, WHITE);
    setBg(cons, RED);
    placeTextVert(cons, "Vertically too!", 4, 5);
    refreshConsole(cons);
    sleep(3);
    setFg(cons, RESET_COLOUR);
    setBg(cons, RESET_COLOUR);

    clearCanvas(cons);
    placeTextHor(cons, "You can draw characters to the console", 2, 5);
    refreshConsole(cons);
    setFg(cons, BYELLOW);
    sleep(1);
    for (int i=0; i<cons->maxX; i++) {
        inputChar(cons, '*', i, 0);
        refreshConsole(cons);
        usleep(DELAY);
    }
    for (int i=0; i<cons->maxY; i++) {
        inputChar(cons, '*', cons->maxX-1, i);
        refreshConsole(cons);
        usleep(DELAY); 
    }
    for (int i=cons->maxX-1; i>=0; i--) {
        inputChar(cons, '*', i, cons->maxY-1);
        refreshConsole(cons);
        usleep(DELAY);
    }
    for (int i=cons->maxY-1; i>=0; i--) {
        inputChar(cons, '*', 0, i);
        refreshConsole(cons);
        usleep(DELAY);
    }
    setFg(cons, RESET_COLOUR);
    clearCanvas(cons);
    placeTextHor(cons, "Or you could draw some lines", 2, 5);
    refreshConsole(cons);
    sleep(1);

    clearCanvas(cons);
    setFg(cons, BBLUE);
    drawLine(cons, '#', 0, 11, 9, 0);
    refreshConsole(cons);
    usleep(10 * DELAY);
    setFg(cons, BRED);
    drawLine(cons, '#', 9, 0, 20, 9);
    refreshConsole(cons);
    usleep(10 * DELAY);
    setFg(cons, BGREEN);
    drawLine(cons, '#', 20, 9, 11, 20);
    refreshConsole(cons);
    usleep(10 * DELAY);
    setFg(cons, BYELLOW);
    drawLine(cons, '#', 0, 11, 11, 20);
    refreshConsole(cons);
    usleep(10 * DELAY);
    setFg(cons, WHITE);
    drawLine(cons, 'X', 0, 0, 20, 20);
    refreshConsole(cons);
    usleep(10 * DELAY);
    setFg(cons, WHITE);
    drawLine(cons, 'X', 20, 0, 0, 20);
    setFg(cons, RESET_COLOUR);
    setBg(cons, RESET_COLOUR);
    refreshConsole(cons);
    sleep(1);

    clearCanvas(cons);
    placeTextHor(cons, "Or how about a sprite?", 2, 5);
    refreshConsole(cons);
    sleep(1);

    #define ROW 4
    #define COL 5
    char smiley [ROW * COL] = {
        ' ', '|', ' ', '|', ' ',
        ' ', '|', ' ', '|', ' ',
        ' ', ' ', ' ', ' ', ' ',
        '\\', '_', '_', '_', '/'
    };

    clearCanvas(cons);
    drawSprite(cons, smiley, ROW, COL, 10, 10);
    refreshConsole(cons);
    usleep(25 * DELAY);
    clearCanvas(cons);
    drawSprite(cons, smiley, ROW, COL, 2, 4);
    refreshConsole(cons);
    usleep(25 * DELAY);
    clearCanvas(cons);
    drawSprite(cons, smiley, ROW, COL, 15, 9);
    refreshConsole(cons);

    deleteCanvas(cons);
    
    return 0;
}