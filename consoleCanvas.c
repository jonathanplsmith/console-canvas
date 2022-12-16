#include "consoleCanvas.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

canvas *newCanvas(int maxX, int maxY) {
    canvas *curr = malloc(sizeof(canvas));
    if (!curr) return NULL;

    curr->maxX = maxX;
    curr->maxY = maxY;
    curr->vals = (char *) malloc(sizeof(char) * maxX * 2 * maxY);
    if (!curr->vals) {
        free(curr);
        return NULL;
    }

    for (int i=0; i < maxX * maxY * 2; i++) {
        curr->vals[i] = ' ';
    }
    return curr;

}

//assumes index locations are in range!
static inline char getChar(canvas *curr, int x, int y) {
    return curr->vals[2 * y * curr->maxX + x];
}
static inline char getCharRaw(canvas *curr, int x, int yR) {
    return curr->vals[yR * curr->maxX + x];
}
static inline void setChar(canvas *curr, int x, int y, char val) {
    curr->vals[2 * y * curr->maxX + x] = val;
}
static inline void setCharRaw(canvas *curr, int x, int yR, char val) {
    curr->vals[yR * curr->maxX + x] = val;
}


void deleteCanvas(canvas *curr) {
    free(curr->vals);
    free(curr);
}

void refreshConsole(canvas *curr) {
    //OS specific clear console
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(__WIN32__)
        system("cls");
    #endif

    int maxX = curr->maxX;
    int maxY = curr->maxY;

    for (int x=0; x < maxX; x++) {
        for (int yR=0; yR < 2*maxY; yR++) { //here we need to actually traverse the raw array hence yR = yRaw
            printf("%c", getCharRaw(curr, x, yR));
        }
        printf("\n");
    }
}

void clearCanvas(canvas *curr) {
    int maxX = curr->maxX;
    int maxY = curr->maxY;

    for (int i=0; i < maxX * maxY * 2; i++) {
        curr->vals[i] = ' ';
    }
    refreshConsole(curr);
}

bool outOfBounds(canvas *curr, int x, int y) {
    return y < 0 || x < 0 || x >= curr->maxX || y >= curr->maxY;
}

bool inputChar(canvas *curr, char input, int x, int y) {
    int maxX = curr->maxX;
    int maxY = curr->maxY;

    if (outOfBounds(curr, x, y)) 
        return false;
    
    setChar(curr, x, y, input);
    return true;
}

bool drawLine(canvas *curr, char type, int startX, int startY, int endX, int endY) {
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

bool placeTextVert(canvas *curr, char text[], int startX, int startY) {
    int len = strlen(text);
    int maxX = curr->maxX;
    int maxY = curr->maxY;

    for (int i=0; i<len; i++) {
        if (outOfBounds(curr, startX+i, startY))
            return false;
        setChar(curr, startX+i, startY, text[i]);
    }

    return true;
}

bool placeTextHor(canvas *curr, char text[], int startX, int startY) {
    if (outOfBounds(curr, startX, startY))
        return false;
    
    int len = strlen(text);
    int maxX = curr->maxX;
    int maxY = curr->maxY;

    for (int i=0; i<len; i++) {
        if (startY + i >= 2*curr->maxY-1)
            return false;
        //Because we don't want the text to have spaces in it, we access the raw array
        setCharRaw(curr, startX, startY+i, text[i]);
    }

    return true;
}

bool drawSprite(canvas *curr, char *sprite, int rows, int cols, int startX, int startY) {
    bool succ = true;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            succ = succ && inputChar(curr, sprite[i * cols + j], startX + i, startY + j);
        }
    }
    return succ;
} 