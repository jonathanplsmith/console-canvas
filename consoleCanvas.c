#include "consoleCanvas.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

canvas newCanvas(int height, int width) {
    canvas curr;
    curr.height = height;
    curr.width = width;
    curr.vals = (char**) malloc(sizeof(char*) * height);

    for (int i=0; i<height; i++) {
        curr.vals[i] = (char*) malloc(sizeof(char) * 2 * width);
    }
    for (int i=0; i < height; i++) {
        for (int j=0; j < 2 * width; j++) {
            curr.vals[i][j] = ' ';
        }
    }
    return curr;
}

void deleteCanvas(canvas* curr) {
    for (int i=0; i<curr->height; i++) {
        free(curr->vals[i]);
    }
    free(curr->vals);
}

void refreshConsole(canvas* curr) {
    //OS specific clear console
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(__WIN32__)
        system("cls");
    #endif

    for (int i=0; i < curr->height; i++) {
        for (int j=0; j < 2 * curr->width; j++) {
            printf("%c", curr->vals[i][j]);
        }
        printf("\n");
    }
}

void clearCanvas(canvas* curr) {
    for (int i=0; i < curr->height; i++) {
        for (int j=0; j < 2 * curr->width; j++) {
            curr->vals[i][j] = ' ';
        }
    }
    refreshConsole(curr);
}

bool outOfBounds(canvas* curr, int x, int y) {
    return y < 0 || x < 0 || x >= curr->height || y >= curr->width;
}

static inline void inputCharUnsafe(canvas* curr, char input, int x, int y) {
    curr->vals[x][2 * y] = input;
}

bool inputChar(canvas* curr, char input, int x, int y) {
    if (outOfBounds(curr, x, y)) 
        return false;
    
    curr->vals[x][2 * y] = input;
    return true;
}

bool drawLine(canvas* curr, char type, int startX, int startY, int endX, int endY) {
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

bool placeTextVert(canvas* curr, char text[], int startX, int startY) {
    int len = strlen(text);

    for (int i=0; i<len; i++) {
        if (startX + i >= 2*curr->height-1)
            return false;
        curr->vals[startX+i][startY] = text[i];
    }

    return true;
}

bool placeTextHor(canvas* curr, char text[], int startX, int startY) {
    int len = strlen(text);

    for (int i=0; i<len; i++) {
        if (startY + i >= 2*curr->width-1)
            return false;
        curr->vals[startX][startY+i] = text[i];
    }

    return true;
}

bool drawSprite(canvas* curr, char **sprite, int rows, int cols, int startX, int startY) {
    bool succ = true;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            succ = succ && inputChar(curr, sprite[i][j], startX + i, startY + j);
        }
    }
} 