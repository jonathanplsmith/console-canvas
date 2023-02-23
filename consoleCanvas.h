#ifndef CONSOLE_CANVAS
#define CONSOLE_CANVAS

#include <stdbool.h>

typedef enum __attribute((__packed__))__ {
    RESET_COLOUR = 39,
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    BBLACK = 90,
    BRED = 91,
    BGREEN = 92,
    BYELLOW = 93,
    BBLUE = 94,
    BMAGENTA = 95,
    BCYAN = 96,
    BWHITE = 97
} colour_t;

/* The escape code for a background colour is always the code for the foreground colour + 10.
    colour_t saves the codes for foreground colours. */
#define BG_OFFSET 10

struct field_data {
    char val;
    colour_t fg;
    colour_t bg;
};

typedef struct {
    int maxX;
    int maxY;
    colour_t currFg;
    colour_t currBg;
    struct field_data *vals;
} canvas_t;

/* Returns a new canvas of size maxX * maxY. The horizontal direction in the console is the 
    positive y-axis; the vertical direction is the positive x-axis, st. the origin is in the
    top left corner of the console.
    Example for (x, y):
    (0, 0) ---------------------> (0, maxY-1)
    |
    |
    |
    |
    |
    V
    (maxX-1, 0)
    
*/
canvas_t *newCanvas(int maxX, int maxY);

/* Frees the memory associated with the passed canvas_t. */
void deleteCanvas(canvas_t *curr);

/* Draws the passed canvas_t to the console. */
void refreshConsole(canvas_t *curr);

/* Clears the passed canvas_t (returns it to default state). */
void clearCanvas(canvas_t *curr);

/* Checks if the passed coordinates are out of bounds of the passed canvas_t. */
bool outOfBounds(canvas_t *curr, int x, int y);

/* Places char input at the passed coordinates on the passed canvas_t.
    Returns true on success and false on out-of-bounds. */
bool inputChar(canvas_t *curr, char input, int x, int y);

/* Draws a line of char type between (startX, startY) and (endX, endY) on the passed canvas_t.
    Returns true if no point of the line was out of bounds, else returns false. */
bool drawLine(canvas_t *curr, char type, int startX, int startY, int endX, int endY);

/* Writes the given text horizontally starting at (startX, startY) onto the passed canvas_t.
    Uniquely, a new character is drawn onto every new y-Axis position, not every second,
    so the text doesn't look streched out.
    Returns true if no character is out of bounds, else returns false. */
bool placeTextVert(canvas_t *curr, const char text[], int startX, int startY);

/* Writes the given text vertically starting at (startX, startY) onto the passed canvas_t.
    Returns true if no character is out of bounds, else returns false. */
bool placeTextHor(canvas_t *curr, const char text[], int startX, int startY);

/* Draws the given sprite onto the passed canvas_t, where sprite[0] will be drawn onto
    canvas[startX][startY] with fg colour fg[0] and bg colour bg[0]. 
    Assumes that sprite/fg/bg is saved in row-major order!
    If fg/bg is NULL, then the currently active colour will be used instead.
    Returns true if no character is out of bounds, else returns false. */
bool drawSprite(canvas_t *curr, const char *sprite, const colour_t *fg, const colour_t *bg, 
             int rows, int cols, int startX, int startY);

/* Sets the active foreground colour to that specified in fg. All subsequent new elements added to
    the canvas with the exception of sprites will have the active foreground colour. 
    RESET_COLOUR returns to the default system foreground colour. */
void setFg(canvas_t *curr, colour_t fg);

/* Sets the active background colour to that specified in bg. All subsequent new elements added to
    the canvas with the exception of sprites will have the active background colour. 
    RESET_COLOUR returns to the default system background colour. */
void setBg(canvas_t *curr, colour_t bg);

/* Sets the background colour of all tiles to bg. 
    The currently active background colour will NOT be changed. */
void paintBg(canvas_t *curr, colour_t bg);

/* Sets the foreground colour of all tiles to fg. 
    The currently active foreground colour will NOT be changed. */
void paintFg(canvas_t *curr, colour_t fg);

#endif