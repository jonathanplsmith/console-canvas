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

    if (maxX <= 0 || maxY <= 0) return NULL;

    canvas_t *curr = malloc(sizeof(canvas_t));
    if (!curr) return NULL;

    curr->maxX = maxX;
    curr->maxY = maxY;
    curr->currFg = RESET_COLOUR;
    //bg escape chars are always offset compared to fg escape chars
    curr->currBg = RESET_COLOUR + BG_OFFSET; 

    size_t elems = maxX * maxY; 
    curr->vals = (struct field_data *) malloc(sizeof(struct field_data) * elems);
    if (!curr->vals) {
        free(curr);
        return NULL;
    }
    for (unsigned int i=0; i < elems; i++) {
        curr->vals[i].val = ' ';
        curr->vals[i].fg = RESET_COLOUR;
        curr->vals[i].bg = RESET_COLOUR + 10; //see explanation above
    }

    return curr;
}

//assumes index locations are in range!
static inline void printCharRaw(canvas_t *curr, int x, int y) {
    //Retrieve colours and set the terminal colours via escape chars, restore to default after
    colour_t XYfg = curr->vals[x * curr->maxY + y].fg;
    colour_t XYbg = curr->vals[x * curr->maxY + y].bg;
    printf("\033[%dm\033[%dm", XYfg, XYbg + BG_OFFSET);
    printf("%c", curr->vals[x * curr->maxY + y].val);
    printf("\033[%dm\033[%dm", RESET_COLOUR, RESET_COLOUR + BG_OFFSET);
}
static inline void setCharRaw(canvas_t *curr, int x, int y, char val) {
    curr->vals[x * curr->maxY + y].val = val;
    curr->vals[x * curr->maxY + y].fg = curr->currFg;
    curr->vals[x * curr->maxY + y].bg = curr->currBg;
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
        for (int y=0; y < maxY; y++) {
            printCharRaw(curr, x, y);
        }
        printf("\n");
    }
}

void clearCanvas(canvas_t *curr) {
    size_t elems = curr->maxX * curr->maxY;

    for (unsigned int i=0; i < elems; i++) {
        curr->vals[i].val = ' ';
        curr->vals[i].fg = RESET_COLOUR;
        curr->vals[i].bg = RESET_COLOUR + BG_OFFSET;
    }
    refreshConsole(curr);
}

bool outOfBounds(canvas_t *curr, int x, int y) {
    return x < 0 || x >= curr->maxX || y < 0 || y >= curr->maxY;
}

bool inputChar(canvas_t *curr, char input, int x, int y) {
    if (outOfBounds(curr, x, y)) 
        return false;
    
    setCharRaw(curr, x, y, input);
    return true;
}

//Implementation of the Bresenham Line Algorithm courtesy of Wikipedia
bool drawLine(canvas_t *curr, char type, int startX, int startY, int endX, int endY) {
    /* to account for the fact that character fields aren't sqaure, 
        we move 2y for every x to make the line somewhat straight. 
        This isn't a great solution so it should eventually be improved... */
    startY /= 2;
    endY /= 2;
    
    bool succ = true;
    int dx =  abs(endX - startX);
    int stepX = startX < endX ? 1 : -1;
    int dy = -abs(endY - startY);
    int stepY = startY < endY ? 1 : -1;
    int error = dx + dy;

    while (true) {
        succ = succ & inputChar(curr, type, startX, 2*startY); //afformentioned bodge-solution
        refreshConsole(curr);
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

bool placeTextVert(canvas_t *curr, const char text[], int startX, int startY) {   
    int len = strlen(text);

    for (int i=0; i<len; i++) {
        if (outOfBounds(curr, startX+i, startY))
            return false;
        setCharRaw(curr, startX+i, startY, text[i]);
    }

    return true;
}

bool placeTextHor(canvas_t *curr, const char text[], int startX, int startY) {
    if (outOfBounds(curr, startX, startY))
        return false;
    
    int len = strlen(text);

    for (int i=0; i<len; i++) {
        if (startY + i >= curr->maxY)
            return false;
        setCharRaw(curr, startX, startY+i, text[i]);
    }

    return true;
}

bool drawSprite(canvas_t *curr, const char *sprite, const colour_t *fg, const colour_t *bg,
                int rows, int cols, int startX, int startY) {
    bool succ = true;
    colour_t prev_fg = curr->currFg; //restore active colour after call
    colour_t prev_bg = curr->currBg;
    bool custom_fg = fg != NULL; //if colour == NULL, use active colour
    bool custom_bg = bg != NULL;

    for (int i=0; i<rows; i++) {
        int icols = i * cols;
        for (int j=0; j<cols; j++) {
            curr->currFg = custom_fg ? fg[icols + j] : prev_fg;
            curr->currBg = custom_bg ? bg[icols + j] : prev_bg;
            succ = succ & inputChar(curr, sprite[icols + j], startX + i, startY + j);
        }
    }
    curr->currFg = prev_fg;
    curr->currBg = prev_bg;

    return succ;
} 

void setFg(canvas_t *curr, colour_t fg) {
    curr->currFg = fg;
}

void setBg(canvas_t *curr, colour_t bg) {
    curr->currBg = bg;
}

void paintBg(canvas_t *curr, colour_t bg) {
    size_t elems = curr->maxX * curr->maxY;

    for (unsigned int i=0; i<elems; i++) {
        curr->vals[i].bg = bg;
    }
}

void paintFg(canvas_t *curr, colour_t fg) {
    size_t elems = curr->maxX * curr->maxY;

    for (unsigned int i=0; i<elems; i++) {
        curr->vals[i].fg = fg;
    }
}