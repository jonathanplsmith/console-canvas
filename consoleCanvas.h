#ifndef CONSOLE_CANVAS
#define CONSOLE_CANVAS

#include <stdbool.h>

typedef struct {
    int maxX;
    int maxY;
    char** vals;
} canvas;

/* Returns a new canvas of size maxX * maxY. The horizontal direction in the console is the 
    positive y-axis; the vertical direction is the positive x-axis, st. the origin is in the
    top left corner of the console.
    Note that for every y-axis pixel two locations are actually allocated, so that
    the canvas appears square */
canvas newCanvas(int maxX, int maxY);

/* Frees the memory associated with the passed canvas */
void deleteCanvas(canvas* curr);

/* Draws the passed canvas to the console */
void refreshConsole(canvas* curr);

/* Clears the passed canvas by replacing all chars with ' ' */
void clearCanvas(canvas* curr);

/* Checks if the passed coordinates are out of bounds of the passed canvas */
bool outOfBounds(canvas* curr, int x, int y);

/* Places char input at the passed coordinates on the passed canvas.
    Returns true on success and false on out-of-bounds */
bool inputChar(canvas* curr, char input, int x, int y);

/* Draws a line of char type between (startX, startY) and (endX, endY) on the passed canvas.
    Returns true if no point of the line was out of bounds, else returns false */
bool drawLine(canvas* curr, char type, int startX, int startY, int endX, int endY);

/* Writes the given text horizontally starting at (startX, startY) onto the passed canvas.
    Uniquely, a new character is drawn onto every new y-Axis position, not every second,
    so the text doesn't look streched out.
    Returns true if no character is out of bounds, else returns false */
bool placeTextVert(canvas* curr, char text[], int startX, int startY);

/* Writes the given text vertically starting at (startX, startY) onto the passed canvas.
    Returns true if no character is out of bounds, else returns false */
bool placeTextHor(canvas* curr, char text[], int startX, int startY);

/* Draws the given 2d-char array onto the passed canvas , where sprite[0][0] will be drawn onto
    canvas[startX][startY].
    Returns true if no character is out of bounds, else returns false */
bool drawSprite(canvas* curr, char **sprite, int rows, int cols, int startX, int startY);

#endif