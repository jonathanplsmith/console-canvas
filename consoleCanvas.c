#include "consoleCanvas.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

canvas_t *newCanvas(int maxX, int maxY) {
    #if !(defined(__linux__) || defined(__unix__) || defined(__APPLE__))
        printf("Your OS is currently not supported :(\n");
        exit(1);
    #endif

    canvas_t *curr = malloc(sizeof(canvas_t));
    if (!curr) return NULL;

    curr->maxX = maxX;
    curr->maxY = maxY;
    curr->currFg = RESET_COLOUR;
    curr->currBg = RESET_COLOUR + BG_OFFSET; //bg escape chars are always offset compared to fg escape chars

    curr->vals = (struct field_data *) malloc(sizeof(struct field_data) * maxX * 2 * maxY);
    if (!curr->vals) {
        free(curr);
        return NULL;
    }
    for (int i=0; i < maxX * maxY * 2; i++) {
        curr->vals[i].val = ' ';
        curr->vals[i].fg = RESET_COLOUR;
        curr->vals[i].bg = RESET_COLOUR + 10; //see explanation above
    }

    return curr;
}

//assumes index locations are in range!
static inline void printCharRaw(canvas_t *curr, int x, int yR) {
    colour_t XYfg = curr->vals[x * 2 * curr->maxY + yR].fg;
    colour_t XYbg = curr->vals[x * 2 * curr->maxY + yR].bg;
    printf("\033[%dm\033[%dm", XYfg, XYbg + BG_OFFSET);
    printf("%c", curr->vals[x * 2 * curr->maxY + yR].val);
    printf("\033[%dm\033[%dm", RESET_COLOUR, RESET_COLOUR + BG_OFFSET);
}
// static inline void printChar(canvas_t *curr, int x, int y) {
//     printCharRaw(curr, x, 2 * y);
// }
static inline void setCharRaw(canvas_t *curr, int x, int yR, char val) {
    curr->vals[x * 2 * curr->maxY + yR].val = val;
    curr->vals[x * 2 * curr->maxY + yR].fg = curr->currFg;
    curr->vals[x * 2 * curr->maxY + yR].bg = curr->currBg;
}
static inline void setChar(canvas_t *curr, int x, int y, char val) {
    setCharRaw(curr, x, 2 * y, val);
}


void deleteCanvas(canvas_t *curr) {
    //make sure subsequent text in the console isn't coloured
    printf("\033[%dm\033[%dm", RESET_COLOUR, RESET_COLOUR + BG_OFFSET);

    free(curr->vals);
    free(curr);
}

void refreshConsole(canvas_t *curr) {
    //assumes the OS is supported, which is checked in newCanvas
    system("clear");
    
    int maxX = curr->maxX;
    int maxY = curr->maxY;

    for (int x=0; x < maxX; x++) {
        for (int yR=0; yR < 2*maxY; yR++) { //here we need to actually traverse the raw array hence yR = yRaw
            printCharRaw(curr, x, yR);
        }
        printf("\n");
    }
}

void clearCanvas(canvas_t *curr) {
    int maxX = curr->maxX;
    int maxY = curr->maxY;

    for (int i=0; i < maxX * maxY * 2; i++) {
        curr->vals[i].val = ' ';
        curr->vals[i].fg = RESET_COLOUR;
        curr->vals[i].bg = RESET_COLOUR + BG_OFFSET;
    }
    refreshConsole(curr);
}

bool outOfBounds(canvas_t *curr, int x, int y) {
    return y < 0 || x < 0 || x >= curr->maxX || y >= curr->maxY;
}

bool inputChar(canvas_t *curr, char input, int x, int y) {
    if (outOfBounds(curr, x, y)) 
        return false;
    
    setChar(curr, x, y, input);
    return true;
}

bool drawLine(canvas_t *curr, char type, int startX, int startY, int endX, int endY) {
    //Implementation of the Bresenham Line Algorithm courtesy of Wikipedia
    bool succ = true;
    int dx =  abs(endX - startX);
    int stepX = startX < endX ? 1 : -1;
    int dy = -abs(endY - startY);
    int stepY = startY < endY ? 1 : -1;
    int error = dx + dy;

    while (true) {
        succ = succ && inputChar(curr, type, startX, startY);
        if (startX == endX && startY == endY) break;
        int tmp = 2 * error;
        if (tmp > dy) { 
            error += dy; startX += stepX; 
        }
        if (tmp < dx) { 
            error += dx; startY += stepY; 
        }
    }
    
    return succ;
}

bool placeTextVert(canvas_t *curr, char text[], int startX, int startY) {
    int len = strlen(text);

    for (int i=0; i<len; i++) {
        if (outOfBounds(curr, startX+i, startY))
            return false;
        setChar(curr, startX+i, startY, text[i]);
    }

    return true;
}

bool placeTextHor(canvas_t *curr, char text[], int startX, int startY) {
    if (outOfBounds(curr, startX, startY))
        return false;
    
    int len = strlen(text);

    for (int i=0; i<len; i++) {
        if (startY + i >= 2*curr->maxY-1)
            return false;
        //Because we don't want the text to have spaces in it, we access the raw array
        setCharRaw(curr, startX, startY+i, text[i]);
    }

    return true;
}

bool drawSprite(canvas_t *curr, char *sprite, int rows, int cols, int startX, int startY) {
    bool succ = true;
    for (int i=0; i<rows; i++) {
        int icols = i * cols;
        for (int j=0; j<cols; j++) {
            succ = succ && inputChar(curr, sprite[icols + j], startX + i, startY + j);
        }
    }
    return succ;
} 

void setFg(canvas_t *curr, colour_t fg) {
    curr->currFg = fg;
}

void setBg(canvas_t *curr, colour_t bg) {
    curr->currBg = bg;
}