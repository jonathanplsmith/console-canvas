#ifndef CONSOLE_CANVAS
#define CONSOLE_CANVAS

#include <stdbool.h>

typedef struct {
    int height;
    int width;
    char** vals;
} canvas;

canvas newCanvas(int height, int width);

void deleteCanvas(canvas* curr);

void refreshConsole(canvas* curr);

void clearCanvas(canvas* curr);

bool outOfBounds(canvas* curr, int x, int y);

bool inputChar(canvas* curr, char input, int x, int y);

bool drawLine(canvas* curr, char type, int startX, int startY, int endX, int endY);

bool placeTextVert(canvas* curr, char text[], int startX, int startY);

bool placeTextHor(canvas* curr, char text[], int startX, int startY);

bool drawSprite(canvas* curr, char **sprite, int rows, int cols, int startX, int startY);

#endif